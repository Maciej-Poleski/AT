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
    int z = 1;
    cout << z << '\n';
    mt19937 engine(404);
    uniform_int_distribution<char> charDist('a', 'c');
    int length = 100;
    while (z--) {
        for (int i = 0; i < length; ++i) {
            cout << charDist(engine);
        }
        cout << '\n';
    }
}