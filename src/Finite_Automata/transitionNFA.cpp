#include "TransitionNFA.h"
#include <iostream>

TransitionNFA::TransitionNFA(std::set<Node*> beginNodes, std::set<Node*> endNodes, char inputA)
    : beginNodes(beginNodes), endNodes(endNodes), inputA(inputA) {}

TransitionNFA::TransitionNFA(TransitionNFA* refTran)
    : beginNodes(refTran->getBeginNodes()), endNodes(refTran->getEndNodes()), inputA(refTran->getInput()) {}

TransitionNFA::TransitionNFA() : beginNodes({}), endNodes({}), inputA(' ') {}

std::set<Node*> TransitionNFA::getBeginNodes() { return beginNodes; }
void TransitionNFA::setBeginNodes(std::set<Node*> n) { beginNodes = n; }
std::set<Node*> TransitionNFA::getEndNodes() { return endNodes; }
void TransitionNFA::setEndNodes(std::set<Node*> e) { endNodes = e; }
char TransitionNFA::getInput() const { return inputA; }
void TransitionNFA::setInput(char a) { inputA = a; }
bool TransitionNFA::operator==(const TransitionNFA& t) const {
    return beginNodes == t.beginNodes && endNodes == t.endNodes && inputA == t.inputA;
}
TransitionNFA::~TransitionNFA() = default;
