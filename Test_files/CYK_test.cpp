#include "../src/Context_Free/CFG.h"

using namespace std;

int main() {
    freopen("../Output_files/Actual_output/CYK_output.txt","w",stdout);

    CFG cfg("../Input_files/input-cyk1.json");

    cout << "First test results: " << endl;
    cfg.accepts("baaba"); //moet true geven
    cfg.accepts("abba"); //moet false geven

    CFG cfg2("../Input_files/input-cyk2.json");

    cout << "Second test results: " << endl;
    cfg2.accepts("aaabb");
    cfg2.accepts("aaabbb");
    return 0;
}