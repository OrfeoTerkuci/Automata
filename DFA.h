#ifndef AUTOMATA_DFA_H
#define AUTOMATA_DFA_H

#include <iostream>
#include <vector>
#include <set>
#include <map>

class Node;
class transition;
class transitionNFA;
class DFA
{
private:
    // Common components
    std::set<char> alphabet;
    std::set<Node*> nodes;
    std::set<Node*> finalNodes;
    std::set<Node*> beginNodes;
    std::set<transition*> transitions;
    // TFA components
    std::map<std::set<std::string> , bool> table;
    std::set<std::set<Node*>> markedPairs;

private:
    // PA operations
    void evaluate(std::set<std::set<Node*>>&begin , std::set<transitionNFA*> &tempTransitions );
    void eliminateExtra(std::set<transitionNFA*> &currentSet);
    void eliminateExtra(std::set<transition*>&trans);
    // Transition function
    Node* transit(Node* begin , char a);
    std::set<std::set<Node*>> findTransition(std::set<Node*> &beginNodes , char c);
    // TFA operations
    void createTable();
    void fillTable();
public:

    /**
     * @brief Construct a new DFA object from a JSON file
     * 
     * @param filename The path of the JSON file
     */
    DFA(std::string filename);

    /**
     * @brief Construct a new DFA from the product of two other DFA's
     * 
     * @param dfa1 The first DFA
     * @param dfa2 The second DFA
     * @param intersect True if the product is the intersection of the DFA's. False if the union
     */
    DFA(DFA &dfa1 , DFA &dfa2 , bool intersect);

    /**
     * @brief Construct an empty DFA
     * 
     */
    DFA();

    // Getters

    /**
     * @brief Get the Alphabet
     * 
     * @return std::set<char> The Alphabet
     */
    std::set<char> getAlphabet() const;

    /**
     * @brief Getter for the Nodes(States)
     * 
     * @return std::set<Node*> All the states in the DFA
     */
    std::set<Node*> getNodes() const;

    /**
     * @brief Getter for the final states
     * 
     * @return std::set<Node*> All the final states in the DFA
     */
    std::set<Node*> getFinal() const;

    /**
     * @brief Getter for the begin states
     * 
     * @return std::set<Node*> All the final states in the DFA
     */
    std::set<Node*> getBegin() const;

    /**
     * @brief Get the transitions
     * 
     * @return std::set<transition*> All the transitions in the DFA 
     */
    std::set<transition*> getTransitions() const;

    // Setters

    /**
     * @brief Setter for the alphabet
     * 
     */
    void setAlphabet(std::set<char>newAlphabet);

    /**
     * @brief Setter for the states set
     * 
     */
    void setNodes(std::set<Node*>newNodes);

    /**
     * @brief Setter for the final states
     * 
     */
    void setFinal(std::set<Node*>newFinalNodes);

    /**
     * @brief Setter for the begin states
     * 
     */
    void setBegin(std::set<Node*>newBeginNodes);

    /**
     * @brief Setter for the transitions
     * 
     */
    void setTransitions(std::set<transition*>newTransitions);

    // Standard DFA operations
    /**
     * @brief Checks whether a string is accepted by the DFA
     * 
     * @param A The string
     * @return true if the string is accepted
     * @return false if the string is not accepted
     */
    bool accepts(std::string A);

    /**
     * @brief Prints all the components of the DFA (alphabet, states, transitions) in the same format as the input JSON file
     * 
     */
    void print();

    // TFA implementation

    /**
     * @brief Prints the TFA table
     * 
     */
    void printTable();

    /**
     * @brief Minimizes the DFA
     * 
     * @return DFA The minimized DFA
     */
    DFA minimize();

    // Operator overloads

    /**
     * @brief Checks whether two DFA's are equivalent
     * 
     * @param dfa2 The other DFA
     * @return true if equivalent
     * @return false if not equivalent
     */
    bool operator==(DFA &dfa2);

    // Destructor

    ~DFA();
};

#endif //AUTOMATA_DFA_H