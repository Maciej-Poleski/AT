//Maciej Poleski
//#define DEBUG
#ifndef DEBUG
#define NDEBUG
#else
#define _GLIBCXX_CONCEPT_CHECKS
#endif

#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <cassert>
#include <unordered_map>
#include <map>
#include <set>
#include <unistd.h>
#include <fcntl.h>
#include <functional>
#include <unordered_set>
#include <csignal>
#include <list>
#include <cstring>

using namespace std;

static vector<int> MP(const string &data)
{
    const auto n = data.length();
    vector<int> P(n + 1);
    P[0] = -1;
    int t = -1;
    for (unsigned i = 1; i <= n; ++i) {
        while (t > -1 && data[t] != data[i - 1]) {
            t = P[t];
        }
        t += 1;
        assert(t >= 0);
        P[i] = t;
    }
    return P;
}

static unsigned fastExp(unsigned base, unsigned exponent, unsigned modulo)
{
    assert(base > 0);
    assert(exponent >= 0);
    assert(modulo > 0);
    if (exponent == 0) {
        return 1;
    } else if (exponent % 2 == 1) {
        return (static_cast<unsigned long>(base) * fastExp(base, exponent - 1, modulo)) % modulo;
    } else {
        // exponent % 2 == 0
        unsigned long t = fastExp(base, exponent / 2, modulo);
        return (t * t) % modulo;
    }
}

static int sourceSequence(unsigned n, unsigned q)
{
    assert(n > 0);
    assert(q <= 1000000000);
    const unsigned numerator = (fastExp(4, n, 3 * q) + 3UL * q - 1) % (3 * q);
    assert(numerator % 3 == 0);
    return numerator / 3;
}

static int computeCornerCase00(unsigned n, unsigned q)
{
    if (n < 3) {
        return 0;
    }
    return (sourceSequence((n - 1) / 2, q) * (1 + (n % 2 == 0))) % q;
}

static int computeCornerCase11(unsigned n, unsigned q)
{
    if (n == 0) {
        return 0;
    }
    return (computeCornerCase00(n, q) + (n % 2 == 0)) % q;
}

static int computeCornerCase01(unsigned n, unsigned q)
{
    return (computeCornerCase00(n + 1, q) + (n % 2 == 1)) % q;
}

static int computeCornerCase10(unsigned n, unsigned q)
{
    return (computeCornerCase00(n + 1, q)) % q;
}

static int computeCornerCase(char *pattern, unsigned n, unsigned q)
{
    if (pattern[0] == '0') {
        if (pattern[1] == '0') {
            return computeCornerCase00(n, q);
        } else {
            assert(pattern[1] == '1');
            return computeCornerCase01(n, q);
        }
    } else {
        assert(pattern[0] == '1');
        if (pattern[1] == '0') {
            return computeCornerCase10(n, q);
        } else {
            assert(pattern[1] == '1');
            return computeCornerCase11(n, q);
        }
    }
}

static int compute(char *pattern, int length, int n, int q)
{
    if (n == 0) {
        return length == 1 && pattern[0] == '0';
    } else if (n == 1) {
        return length == 1 || (length == 2 && pattern[0] == '0' && pattern[1] == '1');
    } else {
        // n >= 2
        if (length == 1) {
            // wzorzec to 0 lub 1 i oba wystepuja po polowie
            return fastExp(2, n - 1, q);
        } else if (length == 2) {
            return computeCornerCase(pattern, n, q);
        } else {
            // wzorzec ma co najmniej 3 znaki
            int i;
            for (i = 1; i < length; ++i) {
                if (pattern[i - 1] == pattern[i]) {
                    break;
                }
            }
            // 3 mozliwosci: ..010101010.. ..01011010.. .. 010100010...
            if (i == length) {
                // znaki naprzemiennie
                // len=3 redukuje sie do 00 i 11
                if (length == 3) {
                    return (computeCornerCase00(n - 1, q) + computeCornerCase11(n - 1, q)) % q;
                } else if (length == 4) {
                    if (pattern[0] == '0') {
                        return computeCornerCase00(n - 1, q);
                    } else {
                        assert(pattern[0] == '1');
                        return computeCornerCase11(n - 1, q);
                    }
                } else {
                    assert(length >= 5);
                    return 0;
                }
            } else {
                // dwa sasiadujace znaki (lub wiecej, rowniez w innych miejscach)
                bool failed = false; // trzy pod rzad
                int outIt;
                if (i % 2 == 1) {
                    // nieparzysty początek
                    // pierwszy znak na odwrotny
                    pattern[0] = (pattern[0] == '0' ? '1' : '0');
                    outIt = 1;
                    for (int j = 1; j < length; j += 2) {
                        if (pattern[j] == '0') {
                            if (j + 1 < length && pattern[j + 1] == '0') {
                                assert(j >= i);
                                failed = true;
                                break;
                            }
                            pattern[outIt++] = '0';
                        } else {
                            assert(pattern[j] == '1');
                            if (j + 1 < length && pattern[j + 1] == '1') {
                                assert(j >= i);
                                failed = true;
                                break;
                            }
                            pattern[outIt++] = '1';
                        }
                    }
                } else {
                    // parzysty początek
                    outIt = 0;
                    for (int j = 0; j < length; j += 2) {
                        if (pattern[j] == '0') {
                            if (j + 1 < length && pattern[j + 1] == '0') {
                                assert(j >= i);
                                failed = true;
                                break;
                            }
                            pattern[outIt++] = '0';
                        } else {
                            assert(pattern[j] == '1');
                            if (j + 1 < length && pattern[j + 1] == '1') {
                                assert(j >= i);
                                failed = true;
                                break;
                            }
                            pattern[outIt++] = '1';
                        }
                    }
                }
                if (failed) {
                    return 0;
                }
                return compute(pattern, outIt, n - 1, q);
            }

        }
    }
}

int main()
{
//    close(0);
//    open("/home/local/AT/B/testy/t00.in", O_RDONLY);
    ios_base::sync_with_stdio(false);
    char *data = new char[1000001];
    int z;
    cin >> z;
    while (z--) {
        int n, q;
        cin >> n >> q;
        cin >> data;
        cout << compute(data, strlen(data), n, q) << endl;
    }
    delete[] data;
    return 0;
}
