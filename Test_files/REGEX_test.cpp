#include "../src/Finite Automata/RE.h"
#include "../src/Finite Automata/ENFA.h"
#include <iostream>

using namespace std;

int main() {

    //* Test 1
    RE re("(m+y)*+(e+y+m+i)s",'e');
    ENFA enfa = re.toENFA();
    enfa.printStats();
    // Expected true
    cout << boolalpha << enfa.accepts("ys") << endl;
    cout << boolalpha << enfa.accepts("mmyyymmmym") << endl;
    cout << boolalpha << enfa.accepts("s") << endl;

    // Expected false
    cout << boolalpha << enfa.accepts("ss") << endl;
    cout << boolalpha << enfa.accepts("ims") << endl;
    cout << boolalpha << enfa.accepts("mimis") << endl;

    /*
    ! Expected output
    * no_of_states=26
    * no_of_transitions[e]=26
    * no_of_transitions[i]=1
    * no_of_transitions[m]=2
    * no_of_transitions[s]=1
    * no_of_transitions[y]=2
    * degree[0]=1
    * degree[1]=18
    * degree[2]=7
    * true
    * true
    * true
    * false
    * false
    * false
    */

    cout << endl;

    //* Test 2
    RE re2("(kd+x+k)(i)*(k+i)",'x');
    ENFA enfa2 = re2.toENFA();
    enfa2.printStats();

    // Expected true
    cout << boolalpha << enfa2.accepts("kdk") << endl;
    cout << boolalpha << enfa2.accepts("i") << endl;
    cout << boolalpha << enfa2.accepts("kiiiiii") << endl;

    // Expected false
    cout << boolalpha << enfa2.accepts("kikk") << endl;
    cout << boolalpha << enfa2.accepts("") << endl;
    cout << boolalpha << enfa2.accepts("kdiiiiiiki") << endl;
    
    /*
    ! Expected output
    * no_of_states=22
    * no_of_transitions[x]=20
    * no_of_transitions[d]=1
    * no_of_transitions[i]=2
    * no_of_transitions[k]=3
    * degree[0]=1
    * degree[1]=16
    * degree[2]=5
    * true
    * true
    * true
    * false
    * false
    * false
    */
    
    return 0;
}

