#include "../src/Finite_Automata/DFA.h"
using namespace std;

int main() {
    DFA dfa("../Input_files/TFA.json");
    DFA mindfa = dfa.minimize();
    dfa.printTable();
    mindfa.print();
    cout << boolalpha << (dfa == mindfa) << endl;
    return 0;
}