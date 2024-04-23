#include "../src/Finite_Automata/NFA.h"
#include "../src/Finite_Automata/DFA.h"
using namespace std;

int main() {
    NFA nfa("../Input_files/NFA.json");
    nfa.toDFA().print();
    return 0;
}