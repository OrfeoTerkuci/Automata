#ifndef AUTOMATA_PDA_H
#define AUTOMATA_PDA_H
#include <iostream>
#include <string>
#include <vector>
#include <set>
#include "CFG.h"

class State;
class Transition;

class PDA {
    std::set<State*> states;
    std::vector<std::string> alphabet;
    std::vector<std::string> stackAlphabet;
    std::vector<Transition*> transitions;
    State* beginState;
    std::string beginStack;
    std::vector<std::string> stack;

public:

    explicit PDA(const std::string& filename);

    PDA();

    CFG toCFG();

};


#endif //AUTOMATA_PDA_H
