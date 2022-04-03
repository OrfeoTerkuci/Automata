#include <iostream>
#include <vector>
#include <set>
using namespace std;
class Node;
class transition;
class transitionNFA;
class DFA
{
private:
    set<char> alphabet;
    set<Node*> nodes;
    set<Node*> finalNodes;
    set<Node*> beginNodes;
    set<transition*> transitions;
public:
    DFA(string filename);
    DFA(DFA &dfa1 , DFA &dfa2 , bool intersect);
    DFA();
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

    void evaluate(set<set<Node*>>&begin , set<transitionNFA*> &tempTransitions );
    void eliminateExtra(set<transitionNFA*> &currentSet);

    Node* transit(Node* begin , char a);
    bool accepts(string A);
    void print();
    ~DFA();
};
