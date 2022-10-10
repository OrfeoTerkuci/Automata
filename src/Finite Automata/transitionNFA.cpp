#include "transitionNFA.h"
#include <iostream>


transitionNFA::transitionNFA(std::set<Node*> beginNodes, std::set<Node*> endNodes, char inputA) :
                beginNodes(beginNodes) , endNodes(endNodes) , inputA(inputA) {}

transitionNFA::transitionNFA(transitionNFA* refTran) : 
                beginNodes( refTran->getBeginNodes() ),
                endNodes( refTran->getEndNodes() ),
                inputA( refTran->getInput() ) {}

transitionNFA::transitionNFA() : beginNodes({}) , endNodes({}) , inputA(' ') {}

std::set<Node*> transitionNFA::getBeginNodes(){
    return beginNodes;
}
void transitionNFA::setBeginNodes(std::set<Node*> n){
    beginNodes = n;
}
std::set<Node*> transitionNFA::getEndNodes(){
    return endNodes;
}
void transitionNFA::setEndNodes(std::set<Node*> e){
    endNodes = e;
}
char transitionNFA::getInput() const{
    return inputA;
}
void transitionNFA::setInput(char a){
    inputA = a;
    }

transitionNFA::~transitionNFA() = default;
