#include "State.h"

State::State(const std::string &name, bool starting, bool accepting) : name(name), starting(starting),
                                                                       accepting(accepting) {}

State::State() : starting(false) , accepting(false) {}

const std::string &State::getName() const {
    return name;
}

void State::setName(const std::string &newName) {
    State::name = newName;
}

bool State::isStarting() const {
    return starting;
}

void State::setStarting(bool newStarting) {
    State::starting = newStarting;
}

bool State::isAccepting() const {
    return accepting;
}

void State::setAccepting(bool newAccepting) {
    State::accepting = newAccepting;
}

State::~State() = default;
