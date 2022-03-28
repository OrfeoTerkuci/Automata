#include <iostream>
#include <vector>
#include <set>
#include "DFA.h"
using namespace std;

class Node;
class transition;
class transitionNFA;
class ENFA
{
private:
    set<char> alphabet;
    set<Node*> nodes;
    set<Node*> finalNodes;
    set<Node*> beginNodes;
    set<transition*> transitions;
    set<transition*> epsTransitions;
    char eps;
public:
    ENFA(string filename);
    ENFA();
    // Getters
    set<char> getAlphabet() const;
    set<Node*> getNodes() const;
    set<Node*> getFinal() const;
    set<Node*> getBegin() const;
    set<transition*> getTransitions() const;
    char getEps() const;
    // Setters
    void setAlphabet(set<char>newAlphabet);
    void setNodes(set<Node*>newNodes);
    void setFinal(set<Node*>newFinalNodes);
    void setBegin(set<Node*>newBeginNodes);
    void setTransitions(set<transition*>newTransitions);
    void setEps(char newEps);
    // Help functions
    void eliminateExtra(set<transitionNFA*> &currentSet);
    void evaluate(set<set<Node*>> &newNodes , set<transitionNFA*> &tempTransitions);
    set<Node*> transit(set<Node*> begin , char a);
    set<Node*> eclose(set<Node*>);
    bool accepts(string A);
    DFA toDFA();
    void print();
    // Destructor
    ~ENFA();
};