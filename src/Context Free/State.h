#ifndef AUTOMATA_STATE_H
#define AUTOMATA_STATE_H
#include <iostream>

class State {
    std::string name;
    bool starting;
    bool accepting;
public:
    explicit State(const std::string &name, bool starting = false, bool accepting = false);

    State();

    const std::string &getName() const;

    void setName(const std::string &newName);

    bool isStarting() const;

    void setStarting(bool newStarting);

    bool isAccepting() const;

    void setAccepting(bool newAccepting);

    virtual ~State();
};


#endif //AUTOMATA_STATE_H
