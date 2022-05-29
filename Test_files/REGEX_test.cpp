#include "../RE.h"
#include <ios>
using namespace std;

int main() {
    RE re("ab+bc+cdf+e",'e');
    //RE re("ab+ab(c+d)g",'e');
    // RE re("ab(cd)* , 'e");
    // RE re("abc*d" , 'e');
    
    ENFA enfa = re.toENFA();
    enfa.printStats();

    // Test print
    enfa.print();
    // Tests for first regex: simple concatenation and plus
    // Given true
    cout << boolalpha << enfa.accepts("ab") << endl;
    cout << boolalpha << enfa.accepts("bc") << endl;
    cout << boolalpha << enfa.accepts("cdf") << endl;
    cout << boolalpha << enfa.accepts("e") << endl;
    // Given false
    cout << boolalpha << enfa.accepts("a") << endl;
    cout << boolalpha << enfa.accepts("b") << endl;
    cout << boolalpha << enfa.accepts("abc") << endl;
    cout << boolalpha << enfa.accepts("cd") << endl;

    // Real tests
    //RE re("(m+y)*+(e+y+m+i)s",'e');
    // Given true
    // cout << boolalpha << enfa.accepts("ys") << endl;
    // cout << boolalpha << enfa.accepts("mmyyymmmym") << endl;
    // cout << boolalpha << enfa.accepts("s") << endl;

    // Given false
    // cout << boolalpha << enfa.accepts("ss") << endl;
    // cout << boolalpha << enfa.accepts("ims") << endl;
    // cout << boolalpha << enfa.accepts("mimis") << endl;

    return 0;
}

