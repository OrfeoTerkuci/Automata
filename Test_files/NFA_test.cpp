#include "../NFA.h"
using namespace std;

int main() {
    NFA nfa("../Input_files/NFA.json");
    nfa.print();
    // Acceptance tests
    cout << boolalpha << nfa.accepts("") << endl;
    // false
    cout << boolalpha << nfa.accepts("c") << " , " << nfa.accepts("cc") << endl;
    // true , false
    cout << boolalpha << nfa.accepts("ccc") << " , " << nfa.accepts("cccc") << endl;
    // true , true
    cout << boolalpha << nfa.accepts("cccccccccc") << endl;
    // true
    return 0;
}