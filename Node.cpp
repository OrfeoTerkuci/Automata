#include "Node.h"
#include "transition.h"
#include <iostream>
#include <vector>

using namespace std;

Node::Node()
{
    transitions = {};
}

void Node::addTransition(Node* beginNode, Node* endNode, char inputA){
    transition* newTrans = new transition(beginNode,endNode,inputA);
    transitions.push_back(newTrans);
}

Node* Node::transit(Node* begin , char a){
    for(transition* t : transitions){
        if(t->getBeginNode() == begin && t->getInput() == a){
            return t->getEndNode();
        }
    }
    return begin;
}

Node::~Node()
{
}
