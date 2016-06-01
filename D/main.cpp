//Maciej Poleski
//#define DEBUG
#ifndef DEBUG
#define NDEBUG
#else
#define _GLIBCXX_CONCEPT_CHECKS
#endif

#include <iostream>
#include <fstream>
#include <random>
#include <sstream>
#include "compressor.h"

using namespace std;

int main()
{
    mt19937 engine(404);
    uniform_int_distribution<char> charGen(numeric_limits<char>::min(), numeric_limits<char>::max());
//    for (;;) {
//        string data;
//        for (int i = 0; i < 10000000; ++i) {
//            data += charGen(engine);
//        }
//        istringstream compressInput(data);
//        stringstream compressedStream;
//        compress(compressInput, compressedStream);
//        ostringstream uncompressOutput;
//        decompress(compressedStream, uncompressOutput);
//        assert(data == uncompressOutput.str());
//    }
    ifstream input("/home/local/AT/D/testy/test");
    ofstream output("/dev/null");
    compress(input, output);

    return 0;
}
