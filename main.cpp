#include <iostream>
#include "DFA.h"

using namespace std;

int main() {
    DFA dfa("DFA.json");
    cout << boolalpha << dfa.accepts("0010110100") << endl << dfa.accepts("0001") << endl;
    return 0;
}