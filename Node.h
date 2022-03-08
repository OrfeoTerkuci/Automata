#pragma once
#include <iostream>
#include <vector>
using namespace std;
class transition;
class Node
{
private:
    vector<transition*>transitions;
public:
    Node();
    void addTransition(Node* beginNode, Node* endNode, char inputA);
    Node* transit(Node* begin , char a);
    ~Node();
};
