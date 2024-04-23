#ifndef AUTOMATA_ENFA_H
#define AUTOMATA_ENFA_H

#include <set>
#include <string>

class DFA;
class Node;
class transition;
class transitionNFA;

class ENFA {
private:
    std::set<char> alphabet;
    std::set<Node*> nodes;
    std::set<Node*> finalNodes;
    std::set<Node*> beginNodes;
    std::set<transition*> transitions;
    std::set<transition*> epsTransitions;
    char eps;

public:
    explicit ENFA(const std::string& filename);
    explicit ENFA(ENFA*& ref);
    ENFA();
    // Getters
    std::set<char> getAlphabet() const;
    std::set<Node*> getNodes() const;
    std::set<Node*> getFinal() const;
    std::set<Node*> getBegin() const;
    std::set<transition*> getTransitions() const;
    std::set<transition*> getEpsTransitions() const;
    char getEps() const;
    // Setters
    void setAlphabet(std::set<char> newAlphabet);
    void setNodes(std::set<Node*> newNodes);
    void setFinal(std::set<Node*> newFinalNodes);
    void setBegin(std::set<Node*> newBeginNodes);
    void setTransitions(std::set<transition*> newTransitions);
    void setEpsTransitions(std::set<transition*> newEpsTransitions);
    void setEps(char newEps);
    // Help functions
    static void eliminateExtra(std::set<transitionNFA*>& currentSet);
    void evaluate(std::set<std::set<Node*>>& newNodes, std::set<transitionNFA*>& tempTransitions);
    std::set<Node*> transit(const std::set<Node*>& begin, char a);
    std::set<Node*> eclose(std::set<Node*>);
    bool accepts(std::string A);
    DFA toDFA();
    void print();
    void printStats();
    // Destructor
    ~ENFA();
};

#endif // AUTOMATA_ENFA_H
