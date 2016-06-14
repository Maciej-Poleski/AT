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

int main()
{
    ios_base::sync_with_stdio(false);
    int z = 11 * (1 << 10);
    cout << z << '\n';
    {
        int modulo = 10;
        // exhaustive search
        for (int i = 0; i <= 10; ++i) {
            for (int j = 0; j < (1 << 10); ++j) {
                cout << i << ' ' << modulo << '\n';
                for (int k = 9; k >= 0; --k) {
                    cout << ((j & (1 << k)) != 0);
                }
                cout << '\n';
            }
        }
    }
}