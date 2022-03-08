#include "Node.h"
#include "transition.h"
#include <iostream>
#include <vector>

using namespace std;

Node::Node(string name, bool starting, bool accepting)
{
    Node::name = name;
    Node::starting = starting;
    Node::accepting = accepting;
}

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
void Node::setName(string name){
    Node::name = name;
}
string Node::getName()const{
    return Node::name;
}

Node::~Node()
{
}
