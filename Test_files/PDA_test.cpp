#include "../src/Context Free/PDA.h"

using namespace std;

int main() {
//    PDA pda("../Input_files/input-pda2cfg1.json");
//    pda.toCFG().print();
    PDA pda1("../Input_files/input-pda2cfg2.json");
    pda1.toCFG().print();
    return 0;
}

/* Test 1
V = {S, [p,X,p], [p,X,q], [p,Z0,p], [p,Z0,q], [q,X,p], [q,X,q], [q,Z0,p], [q,Z0,q]}
T = {0, 1}
P = {
    S   -> `[q,Z0,p]`
    S   -> `[q,Z0,q]`
    [p,X,p]   -> `1`
    [q,X,p]   -> `0 [q,X,p] [p,X,p]`
    [q,X,p]   -> `0 [q,X,q] [q,X,p]`
    [q,X,p]   -> `1`
    [q,X,q]   -> `0 [q,X,p] [p,X,q]`
    [q,X,q]   -> `0 [q,X,q] [q,X,q]`
    [q,Z0,p]  -> `0 [q,X,p]`
    [q,Z0,q]  -> `0 [q,X,q]`
}
S = S
 */

/* Test 2
V = {S, [p,X,p], [p,X,q], [p,Z0,p], [p,Z0,q], [q,X,p], [q,X,q], [q,Z0,p], [q,Z0,q]}
T = {0, 1}
P = {
    S   -> `[q,Z0,p]`
    S   -> `[q,Z0,q]`
    [p,X,p]     -> `1 [p,X,p] [p,X,p]`
    [p,X,p]     -> `1 [p,X,q] [q,X,p]`
    [p,X,p]     -> ``
    [p,X,q]     -> `1 [p,X,p] [p,X,q]`
    [p,X,q]     -> `1 [p,X,q] [q,X,q]`
    [p,Z0,p]    -> `1`
    [q,X,p]     -> `0 [q,X,p] [p,X,p]`
    [q,X,p]     -> `0 [q,X,q] [q,X,p]`
    [q,X,p]     -> `1 [q,X,p]`
    [q,X,p]     -> ``
    [q,X,q]     -> `0 [q,X,p] [p,X,q]`
    [q,X,q]     -> `0 [q,X,q] [q,X,q]`
    [q,X,q]     -> `1 [q,X,q]`
    [q,Z0,p]    -> `0 [q,X,p] [p,Z0,p]`
    [q,Z0,p]    -> `0 [q,X,q] [q,Z0,p]`
    [q,Z0,q]    -> `0 [q,X,p] [p,Z0,q]`
    [q,Z0,q]    -> `0 [q,X,q] [q,Z0,q]`
}
S = S
 */