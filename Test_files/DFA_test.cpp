#include "../src/Finite_Automata/DFA.h"
using namespace std;

int main() {
    DFA dfa("../Input_files/DFA.json");
    dfa.print();
    cout << boolalpha << dfa.accepts("0010110100") << endl << dfa.accepts("0001") << endl;
    //* Expected output : false , true
    return 0;
}