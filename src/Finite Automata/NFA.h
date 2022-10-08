#ifndef AUTOMATA_NFA_H
#define AUTOMATA_NFA_H

#include <iostream>
#include <vector>
#include <set>
#include "DFA.h"

class Node;
class transition;
class transitionNFA;
class NFA
{
private:
    std::set<char> alphabet;
    std::set<Node*> nodes;
    std::set<Node*> finalNodes;
    std::set<Node*> beginNodes;
    std::set<transition*> transitions;
public:
    NFA(std::string filename);
    NFA();
    // Getters
    std::set<char> getAlphabet() const;
    std::set<Node*> getNodes() const;
    std::set<Node*> getFinal() const;
    std::set<Node*> getBegin() const;
    std::set<transition*> getTransitions() const;
    // Setters
    void setAlphabet(std::set<char>newAlphabet);
    void setNodes(std::set<Node*>newNodes);
    void setFinal(std::set<Node*>newFinalNodes);
    void setBegin(std::set<Node*>newBeginNodes);
    void setTransitions(std::set<transition*>newTransitions);
    // Help functions
    void evaluate(std::set<std::set<Node*>> &newNodes , std::set<transitionNFA*> &tempTransitions);
    void eliminateExtra(std::set<transitionNFA*> &currentSet);
    std::set<Node*> transit(std::set<Node*> begin , char a);
    bool accepts(std::string A);
    DFA toDFA();
    void print();
    // Destructor
    ~NFA();
};

#endif //AUTOMATA_NFA_H