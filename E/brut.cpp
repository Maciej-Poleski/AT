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
//    close(0);
//    open("/home/local/AT/E/testy/t00.in", O_RDONLY);
    ios_base::sync_with_stdio(false);
    int z;
    cin >> z;
    while (z--) {
        string input;
        cin >> input;
        for (int i = 0; i < input.length(); ++i) {
            auto p = min_element(input.data(), input.data() + i + 1, [&input, i](const char &lhs, const char &rhs) {
                string left = string(&lhs, input.data() + i + 1);
                string right = string(&rhs, input.data() + i + 1);
                return left < right;
            }) - input.data();
            cout << p + 1 << ' ';
        }
        cout << '\n';
    }
    return 0;
}