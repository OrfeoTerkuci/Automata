#ifndef AUTOMATA_TRANSITION_H
#define AUTOMATA_TRANSITION_H

#include <iostream>


class Node;
class transition
{
private:
    Node* beginNode;
    Node* endNode;
    char inputA;
public:
    transition(Node* beginNode, Node* endNode, char inputA);
    transition(transition* refTran);
    transition();
    Node* getBeginNode();
    void setBeginNode(Node* n);
    Node* getEndNode();
    void setEndNode(Node* e);
    char getInput();
    void setInput(char a);
    ~transition();
};

#endif //AUTOMATA_TRANSITION_H