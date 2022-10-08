#include "../src/Finite Automata/DFA.h"

using namespace std;

int main() {
    DFA dfa1("../Input_files/Product1.json");
    DFA dfa2("../Input_files/Product2.json");
    DFA product(dfa1,dfa2,true); // true betekent doorsnede, false betekent unie
    product.print();
    return 0;
}