#include "../src/Context_Free/CFG.h"

using namespace std;

int main() {
// Test 1
//    CFG cfg;
//    cfg.print();
//    return 0;

    CFG cfg("../Input_files/CFG.json");
    cfg.print();
    return 0;

}

/*
V = {BINDIGIT, S}
T = {0, 1, a, b}
P = {
    BINDIGIT -> `0`
    BINDIGIT -> `1`
    S -> ``
    S -> `a S b BINDIGIT`
}
S = S
 */