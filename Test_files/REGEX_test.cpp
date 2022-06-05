#include "../RE.h"
#include <ios>
using namespace std;

int main() {
    // Simple regex
    // RE re("ab+bc+cdf+e",'e');
    // ENFA enfa1 = re.toENFA();
    // enfa1.printStats();
    // //* Tests for first regex: simple concatenation and plus
    // // Given true
    // cout << boolalpha << enfa1.accepts("ab") << endl;
    // cout << boolalpha << enfa1.accepts("bc") << endl;
    // cout << boolalpha << enfa1.accepts("cdf") << endl;
    // cout << boolalpha << enfa1.accepts("e") << endl;
    // // Given false
    // cout << boolalpha << enfa1.accepts("a") << endl;
    // cout << boolalpha << enfa1.accepts("b") << endl;
    // cout << boolalpha << enfa1.accepts("abc") << endl;
    // cout << boolalpha << enfa1.accepts("cd") << endl;

    // //TODO Regex with brackets
    // RE re2( "ab+ab(c+d+f)g" , 'e' );
    // ENFA enfa2 = re2.toENFA();
    // enfa2.printStats();
    // //* Tests for second regex: brackets
    // // Given true
    // cout << boolalpha << enfa2.accepts("ab") << endl;
    // cout << boolalpha << enfa2.accepts("abcg") << endl;
    // cout << boolalpha << enfa2.accepts("abdg") << endl;
    // cout << boolalpha << enfa2.accepts("abfg") << endl;
    // //! Given false
    // cout << boolalpha << enfa2.accepts("a") << endl;
    // cout << boolalpha << enfa2.accepts("b") << endl;
    // cout << boolalpha << enfa2.accepts("abc") << endl;
    // cout << boolalpha << enfa2.accepts("e") << endl;

    // //TODO Regex with star
    // RE re3( "abc*d" , 'e' );
    // ENFA enfa3 = re3.toENFA();
    // enfa3.printStats();
    // //* Tests for third regex: star , concatenation and plus
    // // Given true
    // cout << boolalpha << enfa3.accepts("abd") << endl;
    // cout << boolalpha << enfa3.accepts("abcd") << endl;
    // cout << boolalpha << enfa3.accepts("abccd") << endl;
    // cout << boolalpha << enfa3.accepts("abccccccccd") << endl;
    // //! Given false
    // cout << boolalpha << enfa3.accepts("a") << endl;
    // cout << boolalpha << enfa3.accepts("b") << endl;
    // cout << boolalpha << enfa3.accepts("abc") << endl;
    // cout << boolalpha << enfa3.accepts("e") << endl;

    // //TODO Regex with brackets and star
    // RE re4( "ab(cd)*+e" , 'e' );
    // ENFA enfa4 = re4.toENFA();
    // enfa4.printStats();
    // //* Tests for fourth regex: all operations and brackets
    // // Given true
    // cout << boolalpha << enfa4.accepts("ab") << endl;
    // cout << boolalpha << enfa4.accepts("abcd") << endl;
    // cout << boolalpha << enfa4.accepts("abcdcdcdcd") << endl;
    // cout << boolalpha << enfa4.accepts("e") << endl;
    // //! Given false
    // cout << boolalpha << enfa4.accepts("a") << endl;
    // cout << boolalpha << enfa4.accepts("b") << endl;
    // cout << boolalpha << enfa4.accepts("abc") << endl;
    // cout << boolalpha << enfa4.accepts("cd") << endl;

    // // Real tests
    // RE re("(m+y)*+(e+y+m+i)s",'e');
    // ENFA enfa = re.toENFA();
    // // Given true
    // cout << boolalpha << enfa.accepts("ys") << endl;
    // cout << boolalpha << enfa.accepts("mmyyymmmym") << endl;
    // cout << boolalpha << enfa.accepts("s") << endl;

    // // Given false
    // cout << boolalpha << enfa.accepts("ss") << endl;
    // cout << boolalpha << enfa.accepts("ims") << endl;
    // cout << boolalpha << enfa.accepts("mimis") << endl;

    return 0;
}

