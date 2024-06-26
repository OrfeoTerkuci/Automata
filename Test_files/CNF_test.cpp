#include "../src/Context_Free/CFG.h"

using namespace std;

int main() {
    freopen("../Output_files/Actual_output/CNF_output.txt","w",stdout);
    CFG cfg("../Input_files/input-cnf1.json");
    cfg.toCNF();
    cout << endl;
    CFG cfg1("../Input_files/input-cnf2.json");
    cfg1.toCNF();
    return 0;
}