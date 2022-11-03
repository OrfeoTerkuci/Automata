#include "../src/Context Free/CFG.h"

using namespace std;

int main() {
//    freopen("../Output_files/Actual_output/LL1_output.txt","w",stdout);
//    CFG cfg("../Input_files/input-ll1.json");
//    cfg.ll();
//    cout << endl;
    CFG cfg2("../Input_files/input-ll1_2.json");
    cfg2.ll();
    cout << endl;
//    CFG cfg3("../Input_files/input-ll1_3.json");
//    cfg3.ll();
    return 0;
}

/* Test 1
FIRST: {x, }
FOLLOW: {<EOS>, y}
     | x        | y  | <EOS>  |
|----|----------|----|--------|
| S  | `x S y`  |    |        |
|----|----------|----|--------|
 */

/* Test 2
>>> Building LL(1) Table
 >> FIRST:
    A: {c, }
    S: {a, b}
 >> FOLLOW:
    A: {a, b}
    S: {<EOS>, a, b}
>>> Table is built.

-------------------------------------

     | a        | b        | c      | <EOS>  |
|----|----------|----------|--------|--------|
| A  |          |          | `c S`  | <ERR>  |
| S  | `a A b`  | `b A a`  | <ERR>  | <ERR>  |
|----|----------|----------|--------|--------|
 */