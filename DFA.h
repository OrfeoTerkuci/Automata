#include <iostream>
#include <vector>
using namespace std;
class Node;
class transition;
class DFA
{
private:
    vector<char> alphabet;
    vector<Node*> nodes;
    vector<Node*> finalNodes;
    vector<Node*> beginNodes;
    vector<transition*> transitions;
public:
    DFA(string filename);
    DFA();
    // Getters
    vector<char> getAlphabet() const;
    vector<Node*> getNodes() const;
    vector<Node*> getFinal() const;
    vector<Node*> getBegin() const;
    vector<transition*> getTransitions() const;
    // Setters
    void setAlphabet(vector<char>newAlphabet);
    void setNodes(vector<Node*>newNodes);
    void setFinal(vector<Node*>newFinalNodes);
    void setBegin(vector<Node*>newBeginNodes);
    void setTransitions(vector<transition*>newTransitions);

    Node* transit(Node* begin , char a);
    bool accepts(string A);
    void print();
    ~DFA();
};
