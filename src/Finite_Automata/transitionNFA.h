#ifndef AUTOMATA_TRANSITIONNFA_H
#define AUTOMATA_TRANSITIONNFA_H

#include <iostream>
#include <set>

class Node;
class TransitionNFA {
private:
    std::set<Node*> beginNodes;
    std::set<Node*> endNodes;
    char inputA;

public:
    TransitionNFA(std::set<Node*> beginNodes, std::set<Node*> endNodes, char inputA);
    explicit TransitionNFA(TransitionNFA* refTran);
    TransitionNFA();
    std::set<Node*> getBeginNodes();
    void setBeginNodes(std::set<Node*> n);
    std::set<Node*> getEndNodes();
    void setEndNodes(std::set<Node*> e);
    char getInput() const;
    void setInput(char a);

    bool operator==(const TransitionNFA& t) const;

    ~TransitionNFA();
};

#endif // AUTOMATA_TRANSITIONNFA_H