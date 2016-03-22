/*
 * Generuje _rodzinę_ testów.
 */

#include <fstream>
#include <iostream>
#include <sstream>
#include <random>
#include <cassert>

using namespace std;

constexpr int maxN = 1000000;

void emitNotReducedMpKiller(int &testNo, string familyName)
{
    ostringstream sequence;
    for (int i = 1; i < maxN; ++i) {
        sequence << i << ' ';
    }
    string seqStr = sequence.str();
    seqStr.back() = '\n';
    for (char i : {'a', 'd', 'z'}) {
        const auto id = to_string(testNo++);
        ofstream input(familyName + id + ".in");
        ofstream output(familyName + id + ".out");
        input << maxN << '\n';
        input << string(maxN, i) << '\n';
        output << seqStr;
        output << 0 << ' ' << seqStr;
    }
}

constexpr int brutLimit = 1000;
mt19937 engine(404);

string generateRandomString(int size)
{
    assert(size >= 0);
    uniform_int_distribution<char> charGen('a', 'z');
    string result;
    while (size--) {
        result += charGen(engine);
    }
    return result;
}

bool isPeriod(int l, const string &s)
{
    const auto n = s.length();
    assert(l <= n);
    assert(l > 0);
    for (unsigned i = 0; i < n - l; ++i) {
        if (s[i] != s[i + l]) {
            return false;
        }
    }
    return true;
}

void solve(ofstream &output, const string &str)
{
    for (unsigned i = 1; i < str.length(); ++i) {
        if (isPeriod(i, str)) {
            output << i << ' ';
        }
    }
    output << '\n';
    for (unsigned i = 1; i <= str.length(); ++i) {
        int p;
        for (p = i - 1; p > 0; --p) {
            if (isPeriod(p, str.substr(0, i))) {
                break;
            }
        }
        output << p << ' ';
    }
    output << '\n';
}

void emitSmallRandomShortBorder(int &testNo, string familyName)
{
    for (int stepSize: {10, 50, 200}) {
        const auto id = to_string(testNo++);
        ofstream input(familyName + id + ".in");
        ofstream output(familyName + id + ".out");
        uniform_int_distribution<int> stepSizeDistribution(stepSize * 8 / 10, stepSize * 12 / 10);
        string str = generateRandomString(stepSizeDistribution(engine));
        while (str.length() < brutLimit) {
            int borderSize = stepSizeDistribution(engine);
            str += str.substr(0, borderSize);
        }
        input << str.length() << '\n';
        input << str << '\n';
        solve(output, str);
    }
}

void emitSmallRandomLongBorder(int &testNo, string familyName)
{
    for (int i = 0; i < 3; ++i) {
        const auto id = to_string(testNo++);
        ofstream input(familyName + id + ".in");
        ofstream output(familyName + id + ".out");
        uniform_int_distribution<int> initSizeDistribution(5, 15);
        uniform_int_distribution<int> cycleDistribution(0, 4);
        string str = generateRandomString(initSizeDistribution(engine));
        while (str.length() < brutLimit) {
            int cycles = cycleDistribution(engine);
            const int baseLength = str.length();
            int fraction = uniform_int_distribution<int>(0, baseLength)(engine);
            for (int i = 0; i < cycles; ++i)
                str += str.substr(0, baseLength);
            str += str.substr(0, fraction);
        }
        input << str.length() << '\n';
        input << str << '\n';
        solve(output, str);
    }
}

void emitSmallRandomManyCycles(int &testNo, string familyName)
{
    for (int i = 0; i < 3; ++i) {
        const auto id = to_string(testNo++);
        ofstream input(familyName + id + ".in");
        ofstream output(familyName + id + ".out");
        uniform_int_distribution<int> initSizeDistribution(5, 15);
        uniform_int_distribution<int> cycleDistribution(0, 4);
        string str = generateRandomString(initSizeDistribution(engine));
        while (str.length() < brutLimit) {
            int cycles = cycleDistribution(engine);
            const int baseLength = str.length();
            for (int i = 0; i < cycles; ++i)
                str += str.substr(0, baseLength);
        }
        input << str.length() << '\n';
        input << str << '\n';
        solve(output, str);
    }
}

void emitBigRandomShortBorder(int &testNo, string familyName)
{
    for (int i = 0; i < 3; ++i) {
        const auto id = to_string(testNo++);
        ofstream input(familyName + id + ".in");
        uniform_int_distribution<int> initSizeDistribution(5, 15);
        string str = generateRandomString(initSizeDistribution(engine));
        while (str.length() < maxN) {
            int borderSize = uniform_int_distribution<int>(1, str.length())(engine);
            str += str.substr(0, borderSize);
        }
        input << maxN << '\n';
        input << str.substr(0, maxN) << '\n';
    }
}

void emitBigRandomLongBorder(int &testNo, string familyName)
{
    for (int i = 0; i < 3; ++i) {
        const auto id = to_string(testNo++);
        ofstream input(familyName + id + ".in");
        uniform_int_distribution<int> initSizeDistribution(5, 15);
        uniform_int_distribution<int> cycleDistribution(0, 4);
        string str = generateRandomString(initSizeDistribution(engine));
        while (str.length() < maxN) {
            int cycles = cycleDistribution(engine);
            const int baseLength = str.length();
            int fraction = uniform_int_distribution<int>(0, baseLength)(engine);
            for (int i = 0; i < cycles; ++i)
                str += str.substr(0, baseLength);
            str += str.substr(0, fraction);
        }
        input << maxN << '\n';
        input << str.substr(0, maxN) << '\n';
    }
}

void emitBigRandomManyCycles(int &testNo, string familyName)
{
    for (int i = 0; i < 3; ++i) {
        const auto id = to_string(testNo++);
        ofstream input(familyName + id + ".in");
        uniform_int_distribution<int> initSizeDistribution(5, 15);
        uniform_int_distribution<int> cycleDistribution(0, 4);
        string str = generateRandomString(initSizeDistribution(engine));
        while (str.length() < maxN) {
            int cycles = cycleDistribution(engine);
            const int baseLength = str.length();
            for (int i = 0; i < cycles; ++i)
                str += str.substr(0, baseLength);
        }
        input << maxN << '\n';
        input << str.substr(0, maxN) << '\n';
    }
}

int main(int argc, char **argv)
{
    ios_base::sync_with_stdio(false);
    if (argc != 2) {
        clog << argv[0] << " [test name]\n";
        return 1;
    }
    int testNo = 0;
    const string familyName = argv[1];
    emitNotReducedMpKiller(testNo, familyName);
    emitSmallRandomShortBorder(testNo, familyName);
    emitSmallRandomLongBorder(testNo, familyName);
    emitSmallRandomManyCycles(testNo, familyName);
    emitBigRandomShortBorder(testNo, familyName);
    emitBigRandomLongBorder(testNo, familyName);
    emitBigRandomManyCycles(testNo, familyName);
}