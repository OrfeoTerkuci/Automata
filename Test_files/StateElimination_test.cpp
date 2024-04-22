#include "DFA.h"

using namespace std;

int main() {
    DFA dfa("input-state1.json");
    dfa.toRE().print();
    return 0;
}