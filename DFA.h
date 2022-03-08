#include <iostream>
#include <vector>
using namespace std;
class Node;
class DFA
{
private:
    vector<Node*> nodes;
    vector<Node*> finalNodes;
    Node* beginNode;
    vector<char> alphabet;
public:
    DFA();
    bool accepts(string A);
    ~DFA();
};
