#include "NFA.h"
#include "../json.hpp"
#include "DFA.h"
#include "Node.h"
#include "Transition.h"
#include "TransitionNFA.h"
#include <fstream>
#include <iomanip>

using json = nlohmann::json;

NFA::NFA(const std::string& filename) {
    // inlezen uit file
    std::ifstream input(filename);
    json j;
    input >> j;

    for (json::iterator it = j.begin(); it != j.end(); ++it) {
        // Designate alphabet
        if (it.key() == "alphabet") {
            std::vector<std::string> a = it.value();
            for (std::string s : a) {
                alphabet.insert(s[0]);
            }
        }
    }

    // Create the nodes
    auto states = j["states"];
    for (auto state : states) {
        Node* newState = new Node(state["name"], state["starting"], state["accepting"]);
        nodes.insert(newState);
    }
    // Designate beginState and final nodes
    for (Node* n : nodes) {
        if (n->isAccepting()) {
            finalNodes.insert(n);
        }
        if (n->isStarting()) {
            beginNodes.insert(n);
        }
    }
    // Add transitions
    // Create transitions array
    auto ts = j["transitions"];
    Node* beginState;
    Node* endState;
    for (auto t : ts) {
        std::string beginNodeName = t["from"];
        std::string endNodeName = t["to"];
        std::string t_input = t["input"];
        char inputA = t_input[0];
        for (Node* n : nodes) {
            if (n->getName() == beginNodeName) {
                beginState = n;
            }
            if (n->getName() == endNodeName) {
                endState = n;
            }
        }
        auto* newTransition = new Transition(beginState, endState, inputA);
        transitions.insert(newTransition);
    }
}

NFA::NFA() : alphabet({}), nodes({}), beginNodes({}), finalNodes({}), transitions({}) {}

std::set<char> NFA::getAlphabet() const { return NFA::alphabet; }
std::set<Node*> NFA::getNodes() const { return NFA::nodes; }
std::set<Node*> NFA::getFinal() const { return NFA::finalNodes; }
std::set<Node*> NFA::getBegin() const { return NFA::beginNodes; }
std::set<Transition*> NFA::getTransitions() const { return NFA::transitions; }

void NFA::setAlphabet(std::set<char> newAlphabet) { NFA::alphabet = newAlphabet; }
void NFA::setNodes(std::set<Node*> newNodes) { NFA::nodes = newNodes; }
void NFA::setFinal(std::set<Node*> newFinalNodes) { NFA::finalNodes = newFinalNodes; }
void NFA::setBegin(std::set<Node*> newBeginNodes) { NFA::beginNodes = newBeginNodes; }
void NFA::setTransitions(std::set<Transition*> newTransitions) { NFA::transitions = newTransitions; }

std::set<Node*> NFA::transit(const std::set<Node*>& begin, char a) {
    std::set<Node*> c;
    if (alphabet.find(a) == alphabet.end()) {
        return {nullptr};
    }
    for (Transition* t : transitions) {
        for (Node* n : begin) {
            if (t->getBeginNode() == n && t->getInput() == a) {
                c.insert(t->getEndNode());
            }
        }
    }
    return c;
}

void NFA::evaluate(std::set<std::set<Node*>>& newNodes, std::set<TransitionNFA*>& tempTransitions) {
    bool evaluate = true;
    TransitionNFA* newTransition;

    while (evaluate) {
        // Remember old size
        int oldSize = newNodes.size();
        for (std::set<Node*> tempNodes : newNodes) {
            std::set<Node*> oldTemp = tempNodes;
            for (char c : alphabet) {

                // Transit for character c
                tempNodes = transit(tempNodes, c);
                // Add newly acquired set to newNodes
                newNodes.insert(tempNodes);
                if (!tempNodes.empty()) {
                    // Add used transitions
                    newTransition = new TransitionNFA();
                    newTransition->setBeginNodes(oldTemp);
                    // Add endState nodes to transition
                    newTransition->setEndNodes(tempNodes);
                    newTransition->setInput(c);
                    // Add transition to container
                    tempTransitions.insert(newTransition);
                }
                // Reset tempNodes
                tempNodes = oldTemp;
            }
        }
        evaluate = oldSize != newNodes.size();
    }
}

void NFA::eliminateExtra(std::set<TransitionNFA*>& currentSet) {
    for (auto it1 = currentSet.begin(); it1 != currentSet.end(); it1++) {
        for (auto it2 = currentSet.begin(); it2 != currentSet.end(); it2++) {
            if (it1 == it2) {
                continue;
            }
            // Check for duplicate
            auto t1 = *it1;
            auto t2 = *it2;
            std::set<Node*> b1 = t1->getBeginNodes();
            std::set<Node*> e1 = t1->getEndNodes();
            std::set<Node*> b2 = t2->getBeginNodes();
            std::set<Node*> e2 = t2->getEndNodes();
            char c1 = t1->getInput();
            char c2 = t2->getInput();
            if (b1 == b2 && e1 == e2 && c1 == c2) {
                delete t2;
                it2 = currentSet.erase(it2);
            }
        }
    }
}

bool NFA::accepts(std::string A) {
    // Split string into chars
    std::vector<char> v(A.begin(), A.end());
    std::set<Node*> currentNodes = beginNodes;
    for (char inputA : v) {
        currentNodes = transit(currentNodes, inputA);
        if (currentNodes.size() == 1 && *currentNodes.begin() == nullptr) {
            return false;
        }
    }
    for (Node* c : currentNodes) {
        if (c->isAccepting()) {
            return true;
        }
    }
    return false;
}

DFA NFA::toDFA() {
    // Create a new DFA
    DFA dfa;
    // Copy over the alphabet
    dfa.setAlphabet(getAlphabet());
    // Create new containers
    std::set<Node*> dfaNodes;
    std::set<Node*> dfaBegin;
    std::set<Node*> dfaFinalNodes;
    std::set<Transition*> dfaTransitions;
    // Lazy evaluation beginState
    // Create powerset to push to DFA
    std::set<std::set<Node*>> newNodes = {beginNodes};
    // Create temporary transitions container
    std::set<TransitionNFA*> tempTransitions;
    evaluate(newNodes, tempTransitions);
    eliminateExtra(tempTransitions);
    // Create new states
    for (const std::set<Node*>& currentSet : newNodes) {
        int count = 0;
        // Create new node
        Node* newNode = new Node();
        std::string newName;
        bool starting = false;
        bool accepting = false;
        // Create combined name
        newName += "{";
        // Add each nodes name
        for (Node* currentNode : currentSet) {
            // Get new node name
            newName += currentNode->getName();
            if (count != currentSet.size() - 1) {
                newName += ",";
            } else {
                newName += "}";
            }
            // Check if accepting
            if (currentNode->isAccepting()) {
                accepting = true;
            }
            // Check if starting
            if (currentSet == beginNodes) {
                starting = true;
            }
            count++;
        }
        // Check all transitions
        for (TransitionNFA* t : tempTransitions) {
            // Check if transition begins at this set
            if (t->getBeginNodes() == currentSet) {
                t->setBeginNodes({newNode});
            }
            // Check if transition ends at this set
            if (t->getEndNodes() == currentSet) {
                t->setEndNodes({newNode});
            }
        }
        newNode->setName(newName);
        newNode->setStarting(starting);
        newNode->setAccepting(accepting);
        // Insert the node into the DFA
        dfaNodes.insert(newNode);
        // Check if accepting
        if (newNode->isAccepting()) {
            dfaFinalNodes.insert(newNode);
        }
        if (newNode->isStarting()) {
            dfaBegin.insert(newNode);
        }
    }
    // Convert all transitions to dfa transitions
    for (TransitionNFA* t : tempTransitions) {
        dfaTransitions.insert(new Transition(*t->getBeginNodes().begin(), *t->getEndNodes().begin(), t->getInput()));
        delete t;
    }
    // Set all the containters to the dfa
    dfa.setNodes(dfaNodes);
    dfa.setBegin(dfaBegin);
    dfa.setFinal(dfaFinalNodes);
    dfa.setTransitions(dfaTransitions);
    return dfa;
}

void NFA::print() {
    // Make json object
    json j;
    // Set type to NFA
    j["type"] = "NFA";
    // Add alphabet
    // Make temp vector
    std::set<std::string> v_Alphabet;
    for (char c : NFA::alphabet) {
        // Convert char to string and insert into vector
        v_Alphabet.insert(std::string(1, c));
    }
    j["alphabet"] = v_Alphabet;
    // Add states
    json states_array = json::array();
    for (auto s : nodes) {
        // Make new object and add each element of state
        json newObject = json::object();
        newObject["name"] = s->getName();
        newObject["starting"] = s->isStarting();
        newObject["accepting"] = s->isAccepting();
        states_array.push_back(newObject);
    }
    j["states"] = states_array;
    // Add transitions
    json transitions_array = json::array();
    for (auto t : transitions) {
        // Make new object and add each element of transition
        json newObject = json::object();
        newObject["from"] = t->getBeginNode()->getName();
        newObject["to"] = t->getEndNode()->getName();
        newObject["input"] = std::string(1, t->getInput());
        transitions_array.push_back(newObject);
    }
    j["transitions"] = transitions_array;
    // Print to screen
    std::cout << std::setw(4) << j << std::endl;
}

NFA::~NFA() {
    for (auto& n : nodes) {
        delete n;
    }
    for (auto& t : transitions) {
        delete t;
    }
}