#include "../ENFA.h"

using namespace std;

int main() {
    ENFA enfa("../Input_files/ENFA.json");
    enfa.toDFA().print();
    return 0;
}