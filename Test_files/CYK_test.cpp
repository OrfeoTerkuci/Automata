#include "../src/Context Free/CFG.h"

using namespace std;

int main() {
    CFG cfg("../Input_files/input-cyk1.json");

    cout << "First test results: ";
    cfg.accepts("baaba"); //moet true geven
    cfg.accepts("abba"); //moet false geven

    CFG cfg2("../Input_files/input-cyk2.json");

    cout << "Second test results: ";
    cfg2.accepts("aaabb");
    cfg2.accepts("aaabbb");
    return 0;
}