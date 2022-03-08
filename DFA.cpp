#include "Node.h"
#include "DFA.h"
#include <iostream>
using namespace std;

DFA::DFA()
{
    // Designate alphabet
    alphabet = {'0','1'};
    // Create the nodes
    Node* s0 = new Node;
    Node* s1 = new Node;
    Node* s2 = new Node;
    // Add nodes to nodes vector
    nodes = {s0,s1,s2};
    // Designate final nodes
    finalNodes = {s0};
    // Designate begin node
    beginNode = s0;
    // Add transitions
    s0->addTransition(s0,s0,'0');
    s0->addTransition(s0,s1,'1');
    s1->addTransition(s1,s2,'0');
    s1->addTransition(s1,s0,'1');
    s2->addTransition(s2,s1,'0');
    s2->addTransition(s2,s2,'1');
}

bool DFA::accepts(string A){
    // Split string into chars
    vector<char> v(A.begin(),A.end());
    Node* currentNode = beginNode;
    for(char inputA : v){
        currentNode = currentNode->transit(currentNode,inputA);
    }
    for(Node* n : finalNodes){
        if(n == currentNode){
            return true;
        }
    }
    return false;
}

DFA::~DFA()
{

}
