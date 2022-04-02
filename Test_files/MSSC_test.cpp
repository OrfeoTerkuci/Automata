#include "../ENFA.h"

using namespace std;

int main() {
    ENFA enfa("ENFA.json");
    enfa.toDFA().print();
    return 0;
}