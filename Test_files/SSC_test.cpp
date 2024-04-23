#include "../src/Finite Automata/NFA.h"
#include "../src/Finite Automata/DFA.h"
using namespace std;

int main() {
    NFA nfa("../Input_files/NFA.json");
    nfa.toDFA().print();
    return 0;
}