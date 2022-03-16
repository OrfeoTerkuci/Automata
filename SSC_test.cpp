#include "NFA.h"

using namespace std;

int main() {
    NFA nfa("NFA.json");
    nfa.toDFA().print();
    return 0;
}