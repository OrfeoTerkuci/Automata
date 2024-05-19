#include "../src/Context_Free/CFG.h"

using namespace std;

int main() {

    CFG cfg("../Input_files/input-cyk1.json");

    cout << "First test results: " << endl;
    cfg.accepts("baaba"); // True
    cfg.accepts("abba");  // False

    CFG cfg2("../Input_files/input-cyk2.json");

    cout << "Second test results: " << endl;
    cfg2.accepts("aaabb");  // False
    cfg2.accepts("aaabbb"); // True
    return 0;
}