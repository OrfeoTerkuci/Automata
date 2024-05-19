#ifndef AUTOMATA_NFA_H
#define AUTOMATA_NFA_H


#include <iostream>
#include <set>
#include <vector>

class DFA;
class Node;
class Transition;
class TransitionNFA;
class NFA {
private:
    std::set<char> alphabet;
    std::set<Node*> nodes;
    std::set<Node*> finalNodes;
    std::set<Node*> beginNodes;
    std::set<Transition*> transitions;

public:
    explicit NFA(const std::string& filename);
    NFA();
    // Getters
    std::set<char> getAlphabet() const;
    std::set<Node*> getNodes() const;
    std::set<Node*> getFinal() const;
    std::set<Node*> getBegin() const;
    std::set<Transition*> getTransitions() const;
    // Setters
    void setAlphabet(std::set<char> newAlphabet);
    void setNodes(std::set<Node*> newNodes);
    void setFinal(std::set<Node*> newFinalNodes);
    void setBegin(std::set<Node*> newBeginNodes);
    void setTransitions(std::set<Transition*> newTransitions);
    // Help functions
    void evaluate(std::set<std::set<Node*>>& newNodes, std::set<TransitionNFA*>& tempTransitions);
    static void eliminateExtra(std::set<TransitionNFA*>& currentSet);
    std::set<Node*> transit(const std::set<Node*>& begin, char a);
    bool accepts(std::string A);
    DFA toDFA();
    void print();
    // Destructor
    ~NFA();
};

#endif // AUTOMATA_NFA_H