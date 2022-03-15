#include "NFA.h"
using namespace std;

int main() {
    NFA nfa("NFA.json");
    nfa.print();
    return 0;
}