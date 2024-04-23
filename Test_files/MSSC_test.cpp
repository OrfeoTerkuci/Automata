#include "../src/Finite Automata/ENFA.h"
#include "../src/Finite Automata/DFA.h"
using namespace std;

int main() {
    ENFA enfa("../Input_files/ENFA.json");
    enfa.toDFA().print();
    return 0;
}