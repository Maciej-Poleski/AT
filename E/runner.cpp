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
    while (true) {
        system("./gen > killer.in");
        system("./brut < killer.in > killer.out");
        if (system("./solution < killer.in > killer.tout")) {
            cerr << "Wzorcówka zdechła\n";
            break;
        }
        if (system("diff killer.out killer.tout")) {
            cerr << "Błędna adpowiedź\n";
            break;
        }
    }
}