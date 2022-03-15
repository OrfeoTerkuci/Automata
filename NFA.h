#include <iostream>
#include <vector>
using namespace std;

class Node;
class transition;
class NFA
{
private:
    vector<char> alphabet;
    vector<Node*> nodes;
    vector<Node*> finalNodes;
    vector<Node*> beginNodes;
    vector<transition*> transitions;
public:
    NFA(string filename);
    Node* transit(Node* begin , char a);
    bool accepts(string A);
    void print();
    ~NFA();
};