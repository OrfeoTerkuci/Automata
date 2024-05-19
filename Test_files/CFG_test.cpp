#include "../src/Context_Free/CFG.h"

using namespace std;

int main() {
    freopen("../Output_files/Actual_output/CFG_output.txt", "w", stdout);

    CFG cfg("../Input_files/CFG.json");
    cfg.print();
    return 0;

}