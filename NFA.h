#include <iostream>
#include <vector>
#include <set>
#include "DFA.h"
using namespace std;

class Node;
class transition;
class NFA
{
private:
    set<char> alphabet;
    set<Node*> nodes;
    set<Node*> finalNodes;
    set<Node*> beginNodes;
    set<transition*> transitions;
public:
    NFA(string filename);
    NFA();
    // Getters
    set<char> getAlphabet() const;
    set<Node*> getNodes() const;
    set<Node*> getFinal() const;
    set<Node*> getBegin() const;
    set<transition*> getTransitions() const;
    // Setters
    void setAlphabet(set<char>newAlphabet);
    void setNodes(set<Node*>newNodes);
    void setFinal(set<Node*>newFinalNodes);
    void setBegin(set<Node*>newBeginNodes);
    void setTransitions(set<transition*>newTransitions);
    // Help functions
    set<Node*> transit(set<Node*> begin , char a);
    bool accepts(string A);
    DFA toDFA();
    void print();
    // Destructor
    ~NFA();
};