#include "../src/Context_Free/PDA.h"

using namespace std;

int main() {
    freopen("../Output_files/Actual_output/PDA_output.txt", "w", stdout);

    PDA pda("../Input_files/input-pda2cfg1.json");
    pda.toCFG().print();
    PDA pda1("../Input_files/input-pda2cfg2.json");
    pda1.toCFG().print();
    return 0;
}
