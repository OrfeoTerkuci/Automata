#include "../src/Context Free/CFG.h"

using namespace std;

int main() {
    CFG cfg("../Input_files/input-cnf1.json");
    cfg.toCNF();
    CFG cfg1("../Input_files/input-cnf2.json");
    cfg1.toCNF();
    return 0;
}

/* Output Test 1
Original CFG:

V = {A, B, C, D, E, S}
T = {a, b}
P = {
  A -> `C`
  A -> `a`
  B -> `C`
  B -> `b`
  C -> ``
  C -> `C D E`
  D -> `A`
  D -> `B`
  D -> `a b`
  S -> ``
  S -> `a A a`
  S -> `b B b`
}
S = S

-------------------------------------

 >> Eliminating epsilon productions
  Nullables are {A, B, C, D, S}
  Created 15 productions, original had 12

V = {A, B, C, D, E, S}
T = {a, b}
P = {
  A -> `C`
  A -> `a`
  B -> `C`
  B -> `b`
  C -> `C D E`
  C -> `C E`
  C -> `D E`
  C -> `E`
  D -> `A`
  D -> `B`
  D -> `a b`
  S -> `a A a`
  S -> `a a`
  S -> `b B b`
  S -> `b b`
}
S = S

 >> Eliminating unit pairs
  Found 5 unit productions
  Unit pairs: {(A, A), (A, C), (A, E), (B, B), (B, C), (B, E), (C, C), (C, E), (D, A), (D, B), (D, C), (D, D), (D, E), (E, E), (S, S)}
  Created 21 new productions, original had 15

V = {A, B, C, D, E, S}
T = {a, b}
P = {
  A -> `C D E`
  A -> `C E`
  A -> `D E`
  A -> `a`
  B -> `C D E`
  B -> `C E`
  B -> `D E`
  B -> `b`
  C -> `C D E`
  C -> `C E`
  C -> `D E`
  D -> `C D E`
  D -> `C E`
  D -> `D E`
  D -> `a b`
  D -> `a`
  D -> `b`
  S -> `a A a`
  S -> `a a`
  S -> `b B b`
  S -> `b b`
}
S = S

 >> Eliminating useless symbols
  Generating symbols: {A, B, D, S, a, b}
  Reachable symbols: {A, B, S, a, b}
  Useful symbols: {A, B, S, a, b}
  Removed 3 variables, 0 terminals and 15 productions

V = {A, B, S}
T = {a, b}
P = {
  A -> `a`
  B -> `b`
  S -> `a A a`
  S -> `a a`
  S -> `b B b`
  S -> `b b`
}
S = S

 >> Replacing terminals in bad bodies
  Added 0 new variables: {}
  Created 6 new productions, original had 6

V = {A, B, S}
T = {a, b}
P = {
  A -> `a`
  B -> `b`
  S -> `A A`
  S -> `A A A`
  S -> `B B`
  S -> `B B B`
}
S = S

 >> Broke 2 bodies, added 2 new variables
>>> Result CFG:

V = {A, B, S, S_2, S_3}
T = {a, b}
P = {
  A -> `a`
  B -> `b`
  S -> `A A`
  S -> `A S_2`
  S -> `B B`
  S -> `B S_3`
  S_2 -> `A A`
  S_3 -> `B B`
}
S = S
 */

/* Output Test 2
 Original CFG:

V = {A, B, C, D, S}
T = {a, b, c, d}
P = {
    A -> `a A b`
    A -> `a b`
    B -> `c B d`
    B -> `c d`
    C -> `a C d`
    C -> `a D d`
    D -> `b D c`
    D -> `b c`
    S -> `A B`
    S -> `C`
}
S = S

-------------------------------------

 >> Eliminating epsilon productions
    Nullables are {}
    Created 10 productions, original had 10

V = {A, B, C, D, S}
T = {a, b, c, d}
P = {
    A -> `a A b`
    A -> `a b`
    B -> `c B d`
    B -> `c d`
    C -> `a C d`
    C -> `a D d`
    D -> `b D c`
    D -> `b c`
    S -> `A B`
    S -> `C`
}
S = S

 >> Eliminating unit pairs
    Found 1 unit productions
    Unit pairs: {(A, A), (B, B), (C, C), (D, D), (S, C), (S, S)}
    Created 11 new productions, original had 10

V = {A, B, C, D, S}
T = {a, b, c, d}
P = {
    A -> `a A b`
    A -> `a b`
    B -> `c B d`
    B -> `c d`
    C -> `a C d`
    C -> `a D d`
    D -> `b D c`
    D -> `b c`
    S -> `A B`
    S -> `a C d`
    S -> `a D d`
}
S = S

 >> Eliminating useless symbols
    Generating symbols: {A, B, C, D, S, a, b, c, d}
    Reachable symbols: {A, B, C, D, S, a, b, c, d}
    Useful symbols: {A, B, C, D, S, a, b, c, d}
    Removed 0 variables, 0 terminals and 0 productions

V = {A, B, C, D, S}
T = {a, b, c, d}
P = {
    A -> `a A b`
    A -> `a b`
    B -> `c B d`
    B -> `c d`
    C -> `a C d`
    C -> `a D d`
    D -> `b D c`
    D -> `b c`
    S -> `A B`
    S -> `a C d`
    S -> `a D d`
}
S = S

 >> Replacing terminals in bad bodies
    Added 4 new variables: {_a, _b, _c, _d}
    Created 15 new productions, original had 11

V = {A, B, C, D, S, _a, _b, _c, _d}
T = {a, b, c, d}
P = {
    A -> `_a A _b`
    A -> `_a _b`
    B -> `_c B _d`
    B -> `_c _d`
    C -> `_a C _d`
    C -> `_a D _d`
    D -> `_b D _c`
    D -> `_b _c`
    S -> `A B`
    S -> `_a C _d`
    S -> `_a D _d`
    _a -> `a`
    _b -> `b`
    _c -> `c`
    _d -> `d`
}
S = S

 >> Broke 7 bodies, added 7 new variables
>>> Result CFG:

V = {A, A_2, B, B_2, C, C_2, C_3, D, D_2, S, S_2, S_3, _a, _b, _c, _d}
T = {a, b, c, d}
P = {
    A -> `_a A_2`
    A -> `_a _b`
    A_2 -> `A _b`
    B -> `_c B_2`
    B -> `_c _d`
    B_2 -> `B _d`
    C -> `_a C_2`
    C -> `_a C_3`
    C_2 -> `D _d`
    C_3 -> `C _d`
    D -> `_b D_2`
    D -> `_b _c`
    D_2 -> `D _c`
    S -> `A B`
    S -> `_a S_2`
    S -> `_a S_3`
    S_2 -> `D _d`
    S_3 -> `C _d`
    _a -> `a`
    _b -> `b`
    _c -> `c`
    _d -> `d`
}
S = S
 */