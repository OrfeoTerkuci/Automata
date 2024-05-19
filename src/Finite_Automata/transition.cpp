#include "Transition.h"
#include "Node.h"

Transition::Transition(Node* beginNode, Node* endNode, char inputA)
    : beginNode(beginNode), endNode(endNode), inputA(inputA) {}

Transition::Transition(Transition* refTran)
    : beginNode(refTran->getBeginNode()), endNode(refTran->getEndNode()), inputA(refTran->getInput()) {}

Transition::Transition() : beginNode(nullptr), endNode(nullptr), inputA(' ') {}

Node* Transition::getBeginNode() { return beginNode; }
void Transition::setBeginNode(Node* n) { beginNode = n; }
Node* Transition::getEndNode() { return endNode; }
void Transition::setEndNode(Node* e) { endNode = e; }
char Transition::getInput() const { return inputA; }
void Transition::setInput(char a) { inputA = a; }
bool Transition::operator==(const Transition& t) const {
        return beginNode == t.beginNode && endNode == t.endNode && inputA == t.inputA;
    }
Transition::~Transition() = default;
