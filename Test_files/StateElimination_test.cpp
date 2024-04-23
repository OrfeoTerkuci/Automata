#include "../src/Finite_Automata/DFA.h"
#include "../src/Finite_Automata/RE.h"
using namespace std;

int main() {
    DFA dfa("input-state1.json");
    dfa.toRE().print();
    return 0;
}