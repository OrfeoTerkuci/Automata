#ifndef AUTOMATA_TRANSITIONNFA_H
#define AUTOMATA_TRANSITIONNFA_H

#include <iostream>
#include <set>

class Node;
class transitionNFA {
private:
    std::set<Node*> beginNodes;
    std::set<Node*> endNodes;
    char inputA;

public:
    transitionNFA(std::set<Node*> beginNodes, std::set<Node*> endNodes, char inputA);
    explicit transitionNFA(transitionNFA* refTran);
    transitionNFA();
    std::set<Node*> getBeginNodes();
    void setBeginNodes(std::set<Node*> n);
    std::set<Node*> getEndNodes();
    void setEndNodes(std::set<Node*> e);
    char getInput() const;
    void setInput(char a);

    bool operator==(const transitionNFA& t) const;

    ~transitionNFA();
};

#endif // AUTOMATA_TRANSITIONNFA_H