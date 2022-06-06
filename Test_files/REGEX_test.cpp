#include "../RE.h"
#include <iostream>
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

    // // //TODO Regex with brackets and star
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

    // Real tests
    RE re("(m+y)*+(e+y+m+i)s",'e');
    ENFA enfa = re.toENFA();
    enfa.printStats();
    // Given true
    cout << boolalpha << enfa.accepts("ys") << endl;
    cout << boolalpha << enfa.accepts("mmyyymmmym") << endl;
    cout << boolalpha << enfa.accepts("s") << endl;

    // Given false
    cout << boolalpha << enfa.accepts("ss") << endl;
    cout << boolalpha << enfa.accepts("ims") << endl;
    cout << boolalpha << enfa.accepts("mimis") << endl;

    cout << endl;

    //* no_of_states=26
    //* no_of_transitions[e]=26
    //* no_of_transitions[i]=1
    //* no_of_transitions[m]=2
    //* no_of_transitions[s]=1
    //* no_of_transitions[y]=2
    //* degree[0]=1
    //* degree[1]=18
    //* degree[2]=7
    //* true
    //* true
    //* true
    //* false
    //* false
    //* false

    //! Test 2

    // RE re5("(kd+x+k)(i)*(k+i)",'x');
    // ENFA enfa5 = re5.toENFA();
    // enfa5.printStats();

    // // geven true
    // cout << boolalpha << enfa5.accepts("kdk") << endl;
    // cout << boolalpha << enfa5.accepts("i") << endl;
    // cout << boolalpha << enfa5.accepts("kiiiiii") << endl;

    // // geven false
    // cout << boolalpha << enfa5.accepts("kikk") << endl;
    // cout << boolalpha << enfa5.accepts("") << endl;
    // cout << boolalpha << enfa5.accepts("kdiiiiiiki") << endl;

    //* no_of_states=22
    //* no_of_transitions[x]=20
    //* no_of_transitions[d]=1
    //* no_of_transitions[i]=2
    //* no_of_transitions[k]=3
    //* degree[0]=1
    //* degree[1]=16
    //* degree[2]=5
    //* true
    //* true
    //* true
    //* false
    //* false
    //* false

    return 0;
}

