#include "Transition.h"

Transition::Transition(State *begin, State *anEnd,
                       const std::string &input, const std::string &stackTop,
                       const std::vector<std::string> &stackReplacement) : beginState(begin), endState(anEnd), input(input),
                                                                           stackTop(stackTop),
                                                                           stackReplacement(stackReplacement) {}

Transition::Transition() : beginState(nullptr) , endState(nullptr) , stackReplacement({}) {}

State *Transition::getBegin() const {
    return beginState;
}

void Transition::setBegin(State *newBegin) {
    Transition::beginState = newBegin;
}

State *Transition::getEnd() const {
    return endState;
}

void Transition::setEnd(State *newEnd) {
    endState = newEnd;
}

const std::string &Transition::getInput() const {
    return input;
}

void Transition::setInput(const std::string &newInput) {
    Transition::input = newInput;
}

const std::string &Transition::getStackTop() const {
    return stackTop;
}

void Transition::setStackTop(const std::string &newStack) {
    Transition::stackTop = newStack;
}

const std::vector<std::string> &Transition::getStackReplacement() const {
    return stackReplacement;
}

void Transition::setStackReplacement(const std::vector<std::string> &newReplacement) {
    Transition::stackReplacement = newReplacement;
}
