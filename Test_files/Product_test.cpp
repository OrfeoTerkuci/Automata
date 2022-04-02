#include "../DFA.h"

using namespace std;

int main() {
    DFA dfa1("Product1.json");
    DFA dfa2("Product2.json");
    DFA product(dfa1,dfa2,true); // true betekent doorsnede, false betekent unie
    product.print();
    return 0;
}