#include "transition.h"
#include <iostream>
using namespace std;


transition::transition(Node* beginNode, Node* endNode, char inputA)
{
    transition::beginNode = beginNode;
    transition::endNode = endNode;
    transition::inputA = inputA;
}

    Node* transition::getBeginNode(){
        return beginNode;
    }
    void transition::setBeginNode(Node* n){
        beginNode = n;
    }
    Node* transition::getEndNode(){
        return endNode;
    }
    void transition::setEndNode(Node* e){
        endNode = e;
    }
    char transition::getInput(){
        return inputA;
    }
    void transition::setInput(char a){
        inputA = a;
    }

transition::~transition()
{
}
