#ifndef AUTOMATA_TRANSITION_H
#define AUTOMATA_TRANSITION_H

#include <iostream>

class Node;
class Transition {
private:
    Node* beginNode;
    Node* endNode;
    char inputA;

public:
    Transition(Node* beginNode, Node* endNode, char inputA);
    explicit Transition(Transition* refTran);
    Transition();
    Node* getBeginNode();
    void setBeginNode(Node* n);
    Node* getEndNode();
    void setEndNode(Node* e);
    char getInput() const;
    void setInput(char a);

    bool operator==(const Transition& t) const;

    ~Transition();
};

#endif // AUTOMATA_TRANSITION_H