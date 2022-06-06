#include "../RE.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdio.h>
using namespace std;

int main() {
    ifstream ifs("../Input_files/retest2.txt");
    string regex( (std::istreambuf_iterator<char>(ifs) ),
                       (std::istreambuf_iterator<char>()    ) );

    RE re(regex ,'e');
    DFA dfa = re.toENFA().toDFA();
    return 0;
}

