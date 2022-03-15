#include <iostream>
#include <vector>
using namespace std;

class Node;
class transition;
class DFA;
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
    vector<Node*> transit(vector<Node*> begin , char a);
    bool accepts(string A);
    //DFA toDFA();
    void print();
    ~NFA();
};