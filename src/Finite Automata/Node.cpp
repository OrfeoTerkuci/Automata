#include "Node.h"
#include "transition.h"
#include <iostream>
#include <vector>

Node::Node(std::string name, bool starting, bool accepting) : name(name) , starting(starting) , accepting(accepting){}

Node::Node(Node* refNode) : name(refNode->getName()) , starting(refNode->isStarting()) , accepting(refNode->isAccepting()){}

Node::Node() : name("") , starting(false) , accepting(false) {}

void Node::setStarting(bool starting){
    Node::starting = starting;
}
bool Node::isStarting()const{
    return Node::starting;
}
void Node::setAccepting(bool accepting){
    Node::accepting = accepting;
}
bool Node::isAccepting()const{
    return Node::accepting;
}
void Node::setName(std::string name){
    Node::name = name;
}
std::string Node::getName()const{
    return Node::name;
}

Node::~Node()
{
}