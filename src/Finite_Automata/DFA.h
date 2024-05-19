#ifndef AUTOMATA_DFA_H
#define AUTOMATA_DFA_H

#include <iostream>
#include <map>
#include <set>
#include <vector>

class RE;
class Node;
class Transition;
class TransitionNFA;

class DFA {
private:
    // Common components
    std::set<char> alphabet;
    std::set<Node*> nodes;
    std::set<Node*> finalNodes;
    std::set<Node*> beginNodes;
    std::set<Transition*> transitions;
    // TFA components
    std::map<std::set<std::string>, bool> table;
    std::set<std::set<Node*>> markedPairs;

private:
    // PA operations
    void evaluate(std::set<std::set<Node*>>& begin, std::set<TransitionNFA*>& tempTransitions);
    static void eliminateExtra(std::set<TransitionNFA*>& currentSet);
    static void eliminateExtra(std::set<Transition*>& trans);
    // Transition function
    Node* transit(Node* begin, char a);
    std::set<std::set<Node*>> findTransition(std::set<Node*>& beginNodesVector, char c);
    // TFA operations
    void createTable();
    void fillTable();

public:
    /**
     * @brief Construct a new DFA object from a JSON file
     * @param filename The path of the JSON file
     */
    explicit DFA(const std::string& filename);

    /**
     * @brief Construct a new DFA from the product of two other DFAs
     * @param dfa1 The first DFA
     * @param dfa2 The second DFA
     * @param intersect True for the intersection, False for the union
     */
    DFA(DFA& dfa1, DFA& dfa2, bool intersect);

    /**
     * @brief Construct an empty DFA
     */
    DFA();

    // Getters

    /**
     * @brief Get the Alphabet
     * @return std::set<char> The Alphabet
     */
    std::set<char> getAlphabet() const;

    /**
     * @brief Getter for the Nodes(States)
     * @return std::set<Node*> All the states in the DFA
     */
    std::set<Node*> getNodes() const;

    /**
     * @brief Getter for the final states
     * @return std::set<Node*> All the final states in the DFA
     */
    std::set<Node*> getFinal() const;

    /**
     * @brief Getter for the beginState states
     * @return std::set<Node*> All the final states in the DFA
     */
    std::set<Node*> getBegin() const;

    /**
     * @brief Get the transitions
     * @return std::set<Transition*> All the transitions in the DFA
     */
    std::set<Transition*> getTransitions() const;

    // Setters

    /**
     * @brief Setter for the alphabet
     * @param newAlphabet A set of characters for the new alphabet of the DFA
     */
    void setAlphabet(std::set<char> newAlphabet);

    /**
     * @brief Setter for the states set
     * @param newNodes A set of Node pointers for the new states of the DFA
     */
    void setNodes(std::set<Node*> newNodes);

    /**
     * @brief Setter for the final states
     * @param newFinalNodes A set of Node pointers for the new final states of the DFA
     */
    void setFinal(std::set<Node*> newFinalNodes);

    /**
     * @brief Setter for the beginState states
     * @param newBeginNodes A set of Node pointers for the new beginState state(s) of the DFA
     */
    void setBegin(std::set<Node*> newBeginNodes);

    /**
     * @brief Setter for the transitions
     * @param newTransitions A set of Transition pointers for the new transitions in the DFA
     */
    void setTransitions(std::set<Transition*> newTransitions);

    // Standard DFA operations
    /**
     * @brief Checks whether a string is accepted by the DFA
     * @param A The string
     * @return true if the string is accepted
     * @return false if the string is not accepted
     */
    bool accepts(std::string A);

    /**
     * @brief Prints all the components of the DFA (alphabet, states, transitions) in the same format as the input JSON
     * file
     */
    void print();

    // TFA implementation

    /**
     * @brief Prints the TFA table
     */
    void printTable();

    /**
     * @brief Minimizes the DFA
     * @return DFA The minimized DFA
     */
    DFA minimize();

    // Operator overloads

    /**
     * @brief Operator overload for equivalence of two DFAs
     * @param dfa2 The other DFA
     * @return true if equivalent
     * @return false if not equivalent
     */
    bool operator==(DFA& dfa2);

    // Conversion functions
    RE toRE();

    // Destructor

    ~DFA();
};

#endif // AUTOMATA_DFA_H