#include "Node.h"
#include <iostream>

Node::Node(std::string name, bool starting, bool accepting) : name(name) , starting(starting) , accepting(accepting){}

Node::Node(Node* refNode) : name(refNode->getName()) , starting(refNode->isStarting()) , accepting(refNode->isAccepting()){}

Node::Node() : starting(false) , accepting(false) {}

void Node::setStarting(bool newStarting){
    Node::starting = newStarting;
}
bool Node::isStarting()const{
    return Node::starting;
}
void Node::setAccepting(bool newAccepting){
    Node::accepting = newAccepting;
}
bool Node::isAccepting()const{
    return Node::accepting;
}
void Node::setName(std::string newName){
    Node::name = newName;
}
std::string Node::getName()const{
    return Node::name;
}

Node::~Node() = default;
