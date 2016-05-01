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

int main()
{
    //close(0);
    //open("/home/local/AT/A/testy/suite2.in", O_RDONLY);
    ios_base::sync_with_stdio(false);
    int z;
    cin >> z;
    while(z--) {
        string input;
        cin >> input;
        auto P = MP(input);
        auto n = input.length();
        vector<int> lookahead(n+1);
        vector<int> result(n+1);
        for(int i=1;i<=n;++i) {
            lookahead[i] = result[i] = i;
        }
        for(int i=2;i<=n;++i) {
            if(P[i]==0)
                continue; // fail
            if(i - lookahead[result[P[i]]] <= result[P[i]]) { // match
                lookahead[result[P[i]]] = i; // update
                result[i] = result[P[i]];
            }
        }
        cout << result[n] << '\n';
    }
    return 0;
}
