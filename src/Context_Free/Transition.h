#ifndef AUTOMATA_TRANSITION_H
#define AUTOMATA_TRANSITION_H
#include <iostream>
#include <vector>
#include <set>
#include <string>
class State;

class Transition {
    State* beginState;
    State* endState;
    std::string input;
    std::string stackTop;
    std::vector<std::string> stackReplacement;
public:
    Transition(State *begin, State *anEnd, const std::string &input, const std::string &stackTop,
               const std::vector<std::string> &stackReplacement);

    Transition();

    State *getBegin() const;

    void setBegin(State *newBegin);

    State *getEnd() const;

    void setEnd(State *newEnd);

    const std::string &getInput() const;

    void setInput(const std::string &newInput);

    const std::string &getStackTop() const;

    void setStackTop(const std::string &newStack);

    const std::vector<std::string> &getStackReplacement() const;

    void setStackReplacement(const std::vector<std::string> &newReplacement);

};


#endif //AUTOMATA_TRANSITION_H
