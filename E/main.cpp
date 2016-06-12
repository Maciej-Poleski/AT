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

static vector<int> MP(const char *data, int n)
{
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

static void handleChunk(const char *data, int begin, int end)
{
    assert(begin < end);
    auto P = MP(data + begin, end - begin);
    for (int i = 1; i < P.size(); ++i) {
        while (P[P[i]] > 0) {
            P[i] = P[P[i]];
        }
        if (P[i] == 0) {
            cout << begin + 1 << ' ';
        } else {
            cout << begin + i - P[i] + 1 << ' ';
        }
    }
}

static void lyndon(const string &s)
{
    int p = 0;
    int j = 1;
    while (p < s.length()) {
        if (s[j] > s[p]) {
            j += 1;
        } else if (s[j] == s[p]) {
            int i = p;
            while (s[j] == s[i]) {
                i += 1;
                j += 1;
            }
            if (s[j] < s[i]) {
                int m = (j - p) / (j - i);
                for (int k = 0; k < m; ++k) {
//                    cerr << p << ' ' << p + (j - i) << '\n';
                    handleChunk(s.data(), p + (j - i) * k, p + (j - i) * (k + 1));
                }
                p = p + (j - i) * m;
                j = p + 1;
            } else {
                j += 1;
            }
        } else {
//            cerr << p << ' ' << j << '\n';
            handleChunk(s.data(), p, j);
            p = j;
            j += 1;
        }
    }
}

int main()
{
//    close(0);
//    open("/home/local/AT/E/testy/t00.in", O_RDONLY);
    ios_base::sync_with_stdio(false);
    int z;
    cin >> z;
    while (z--) {
        string input;
        cin >> input;
        lyndon(input);
        cout << '\n';
    }
    return 0;
}
