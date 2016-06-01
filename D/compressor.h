#ifndef COMPRESSOR_H
#define COMPRESSOR_H

#ifndef DEBUG
#define NDEBUG
#endif

#include <iostream>
#include <unordered_map>
#include <map>
#include <memory>
#include <cassert>
#include <string>
#include <vector>
#include <cstdint>

namespace {

class CompressDictionary
{
    struct Node
    {
        std::unordered_map<char, std::uint32_t> transitions;

        Node();
    };

    std::vector<Node> data;

public:
    CompressDictionary();

    void *makeIterator()
    {
        return reinterpret_cast<void *>(0);
    }

    unsigned getSize() const
    {
        return data.size();
    }

    unsigned getId(void *iterator)
    {
        auto value = reinterpret_cast<std::ptrdiff_t>(iterator);
        assert(value >= 0);
        return static_cast<unsigned>(value);
    }

    bool isValid(void *iterator)
    {
        return reinterpret_cast<std::ptrdiff_t>(iterator) != -1;
    }

    void *shift(void *iterator, char c);

    void makeNodeAfter(void *iterator, char c);

private:
    Node *asNode(void *iterator)
    {
        return &(data[getId(iterator)]);
    }
};

CompressDictionary::Node::Node()
{
    transitions.max_load_factor(4);
}

CompressDictionary::CompressDictionary() : data(1)
{}

void *CompressDictionary::shift(void *iterator, char c)
{
    Node *node = asNode(iterator);
    auto i = node->transitions.find(c);
    if (i == node->transitions.end()) {
        return reinterpret_cast<void *>(std::ptrdiff_t(-1));
    } else {
        return reinterpret_cast<void *>(std::ptrdiff_t(i->second));
    }
}

void CompressDictionary::makeNodeAfter(void *iterator, char c)
{
    assert(reinterpret_cast<std::ptrdiff_t>(iterator) != -1);
    assert(reinterpret_cast<std::ptrdiff_t>(shift(iterator, c)) == -1);

    data.emplace_back();
    asNode(iterator)->transitions[c] = static_cast<std::uint32_t>(data.size() - 1);
}

class OutputWriter
{
    std::ostream &output;
    std::size_t buffer;
    unsigned end;
public:
    OutputWriter(std::ostream &output) : output(output), buffer(0), end(0)
    {}

    ~OutputWriter();

    void write(std::size_t bits, unsigned size);

private:
    void flush();

};

OutputWriter::~OutputWriter()
{
    flush();
    if (end > 0) {
        output.put(static_cast<char>(buffer & 0xff));
    }
}

void OutputWriter::write(std::size_t bits, unsigned size)
{
    assert(bits < (1 << size));
    if (end + size > sizeof(buffer) * 8) {
        flush();
    }
    assert(end + size <= sizeof(buffer) * 8);
    buffer |= bits << end;
    end += size;
}

void OutputWriter::flush()
{
    while (8 <= end) {
        output.put(static_cast<char>(buffer & 0xff));
        buffer >>= 8;
        end -= 8;
    }
}

class InputReader
{
    std::istream &input;
    std::size_t buffer;
    unsigned end;
public:
    InputReader(std::istream &input) : input(input), buffer(0), end(0)
    {}

    std::size_t read(unsigned size);

private:
    void suck();
};

std::size_t InputReader::read(unsigned size)
{
    assert(size <= 57);
    if (size > end) {
        suck();
    }
    assert(end >= size);
    std::size_t result = buffer & ((1 << size) - 1);
    buffer >>= size;
    end -= size;
    return result;
}

void InputReader::suck()
{
    while (sizeof(buffer) * 8 - end >= 8) {
        auto c = input.get();
        if (c != std::char_traits<char>::not_eof(c)) {
            break;
        }
        buffer |= static_cast<std::size_t>(static_cast<unsigned char>(std::char_traits<char>::to_char_type(c))) << end;
        end += 8;
    }
}

class DecompressDictionary
{
    struct Node
    {
        unsigned previousId;
        char c;
    };
    std::vector<Node> dictionary;

public:
    DecompressDictionary() : dictionary(1)
    {}

    std::size_t size() const
    {
        return dictionary.size();
    }

    bool hasId(std::size_t id) const
    {
        return id < dictionary.size();
    }

    std::string uncompressId(std::size_t id) const;

    void insertTransition(unsigned id, char c);
};

std::string DecompressDictionary::uncompressId(std::size_t id) const
{
    assert(id < dictionary.size());
    std::string result;
    for (; id != 0; id = dictionary[id].previousId) {
        result += dictionary[id].c;
    }
    return std::string(result.rbegin(), result.rend());
}

void DecompressDictionary::insertTransition(unsigned id, char c)
{
    assert(id < dictionary.size());
    dictionary.push_back({id, c});
}

static unsigned log2(std::size_t n)
{
    unsigned result = 0;
    while (n > 1) {
        result += 1;
        n /= 2;
    }
    return result;
}

static unsigned ceilLog2(std::size_t n)
{
    // 000101110110
    // 000001000000
    if ((n & (n - 1)) == 0) {
        return log2(n);
    } else {
        return 1 + log2(n);
    }
}

static void compress(std::istream &input, std::ostream &output)
{
    CompressDictionary dict;
    OutputWriter writer(output);
    using Traits = std::char_traits<char>;

    for (int i = 0; i < 256; ++i) {
        dict.makeNodeAfter(dict.makeIterator(), static_cast<char>(i));
    }

    auto i = dict.makeIterator();
    bool stop = false;
    while (!stop) {
        int c;
        unsigned char byteFromCharacter;
        for (;;) {
            c = input.get();
            if (c != Traits::not_eof(c)) {
                byteFromCharacter = 0;
                stop = true;
            } else {
                byteFromCharacter = static_cast<unsigned char>(Traits::to_char_type(c));
            }
            auto j = dict.shift(i, byteFromCharacter);
            if (!dict.isValid(j) || stop) {
                break;
            } else {
                i = j;
            }
        }
        writer.write(dict.getId(i), ceilLog2(dict.getSize()));
        if (stop && (dict.getId(i) != 0)) {
            writer.write(0, ceilLog2(dict.getSize()));
        }
        if (!stop) {
            dict.makeNodeAfter(i, byteFromCharacter);
            i = dict.shift(dict.makeIterator(), byteFromCharacter);
        }
    }
}

static void decompress(std::istream &input, std::ostream &output)
{
    DecompressDictionary dict;
    InputReader reader(input);
    using Traits = std::char_traits<char>;

    for (int i = 0; i < 256; ++i) {
        dict.insertTransition(0, i);
    }

    auto pk = reader.read(ceilLog2(dict.size()));
    if (pk != 0) {
        output << dict.uncompressId(pk);
    }

    for (; pk != 0;) {
        auto k = reader.read(ceilLog2(dict.size() + 1));

        if (dict.hasId(k)) {
            auto word = dict.uncompressId(k);
            dict.insertTransition(pk, word[0]);
            if (k != 0) {
                output << word;
            }
        } else {
            auto pc = dict.uncompressId(pk);
            dict.insertTransition(pk, pc[0]);
            output << pc << pc[0];
        }
        pk = k;
    }

}

}

class Compressor
{
public:
    Compressor() = default;

    void encode()
    {
        compress(std::cin, std::cout);
    }

    void decode()
    {
        decompress(std::cin, std::cout);
    }
};

#ifndef DEBUG
#undef NDEBUG
#endif

#endif // COMPRESSOR_H
