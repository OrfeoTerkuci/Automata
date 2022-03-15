#include "NFA.h"

using namespace std;

int main() {
    NFA nfa("input-ssc1.json");
    nfa.toDFA().print();
    return 0;
}