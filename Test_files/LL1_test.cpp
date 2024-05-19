#include "../src/Context_Free/CFG.h"

using namespace std;

int main() {
    freopen("../Output_files/Actual_output/LL1_output.txt", "w", stdout);
    CFG cfg("../Input_files/input-ll1.json");
    cfg.ll();
    cout << endl;
    CFG cfg2("../Input_files/input-ll1_2.json");
    cfg2.ll();
    cout << endl;
    CFG cfg3("../Input_files/input-ll1_3.json");
    cfg3.ll();
    return 0;
}
