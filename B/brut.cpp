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

int main()
{
//    close(0);
//    open("/home/local/AT/B/killer.in", O_RDONLY);
//    close(1);
//    open("/home/local/AT/B/killer.out", O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR);
//    ios_base::sync_with_stdio(false);
//    close(2);
//    open("/dev/null", O_WRONLY);
    int z;
    cin >> z;
    while (z--) {
//        cerr << z << ' ';
        int n, q;
        cin >> n >> q;
        string input;
        cin >> input;

        string thueWord = "0";
        for (int i = 0; i < n; ++i) {
            for (size_t j = 0, e = thueWord.size(); j < e; ++j) {
                thueWord += thueWord[j] == '0' ? '1' : '0';
            }
        }

//        cerr << thueWord << '\n';

        int result = 0;
        int lastPosition = -1;
        if (thueWord.size() >= input.size()) {
            for (int i = 0; i < thueWord.size() - input.size() + 1; ++i) {
                bool ok = true;
                for (int j = 0; j < input.size(); ++j) {
                    if ((i + j >= thueWord.size()) || (input[j] != thueWord[i + j])) {
                        ok = false;
                        break;
                    }
                }
                if (ok) {
                    ++result;
//                    std::cerr << '^';
                    if (lastPosition != -1) {
                        if ((lastPosition - i) % 2 != 0) {
                            //cerr << i;
                        }
                    }
                    lastPosition = i;
                } else {
//                    std::cerr << ' ';
                }
            }
        }
//        std::cerr << "\n";
        cout << result % q << '\n';
    }
    return 0;
}
