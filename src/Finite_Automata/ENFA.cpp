#include "ENFA.h"
#include "../json.hpp"
#include "DFA.h"
#include "Node.h"
#include "Transition.h"
#include "TransitionNFA.h"
#include <fstream>
#include <iomanip>

using json = nlohmann::json;

ENFA::ENFA(const std::string& filename) {
    // Read from file
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
        // Get the character for epsilon
        else if (it.key() == "eps") {
            auto a = it.value().begin();
            std::string eps_char = a[0];
            ENFA::eps = eps_char[0];
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
        if (inputA == ENFA::eps) {
            epsTransitions.insert(newTransition);
        }
        transitions.insert(newTransition);
    }
}

ENFA::ENFA() : alphabet({}), eps('*'), nodes({}), beginNodes({}), finalNodes({}), transitions({}), epsTransitions({}) {}

ENFA::ENFA(ENFA*& ref)
    : alphabet(ref->getAlphabet()), eps(ref->getEps()), nodes(ref->getNodes()), beginNodes(ref->getBegin()),
      finalNodes(ref->getFinal()), transitions(ref->getTransitions()), epsTransitions(ref->getEpsTransitions()) {}

std::set<char> ENFA::getAlphabet() const { return ENFA::alphabet; }
std::set<Node*> ENFA::getNodes() const { return ENFA::nodes; }
std::set<Node*> ENFA::getFinal() const { return ENFA::finalNodes; }
std::set<Node*> ENFA::getBegin() const { return ENFA::beginNodes; }
std::set<Transition*> ENFA::getTransitions() const { return ENFA::transitions; }

std::set<Transition*> ENFA::getEpsTransitions() const { return ENFA::epsTransitions; }

char ENFA::getEps() const { return ENFA::eps; }

void ENFA::setAlphabet(std::set<char> newAlphabet) { ENFA::alphabet = newAlphabet; }
void ENFA::setNodes(std::set<Node*> newNodes) { ENFA::nodes = newNodes; }
void ENFA::setFinal(std::set<Node*> newFinalNodes) { ENFA::finalNodes = newFinalNodes; }
void ENFA::setBegin(std::set<Node*> newBeginNodes) { ENFA::beginNodes = newBeginNodes; }
void ENFA::setTransitions(std::set<Transition*> newTransitions) { ENFA::transitions = newTransitions; }

void ENFA::setEpsTransitions(std::set<Transition*> newEpsTransitions) { ENFA::epsTransitions = newEpsTransitions; }

void ENFA::setEps(char newEps) { ENFA::eps = newEps; }

std::set<Node*> ENFA::transit(const std::set<Node*>& begin, char a) {
    std::set<Node*> c;
    for (Transition* t : transitions) {
        for (Node* n : begin) {
            if (t->getBeginNode() == n && t->getInput() == a) {
                c.insert(t->getEndNode());
            }
        }
    }
    return c;
}

std::set<Node*> ENFA::eclose(std::set<Node*> begin) {
    std::set<Node*> end = begin;
    bool eval = true;
    while (eval) {
        int oldSize = end.size();
        for (Node* n : end) {
            for (Transition* t : epsTransitions) {
                if (t->getBeginNode() == n) {
                    end.emplace(t->getEndNode());
                }
            }
        }
        eval = oldSize != end.size();
    }
    return end;
}

void ENFA::eliminateExtra(std::set<TransitionNFA*>& currentSet) {
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

void ENFA::evaluate(std::set<std::set<Node*>>& newNodes, std::set<TransitionNFA*>& tempTransitions) {
    bool evaluate = true;
    TransitionNFA* newTransition;
    while (evaluate) {
        // Remember old size
        int oldSize = newNodes.size();
        for (std::set<Node*> tempNodes : newNodes) {
            // printNode(tempNodes);
            std::set<Node*> oldTemp = tempNodes;
            for (char c : alphabet) {
                // Transit for character c and eclose
                tempNodes = transit(tempNodes, c);
                tempNodes = eclose(tempNodes);
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

bool ENFA::accepts(std::string A) {
    // Split std::string into chars
    std::vector<char> v(A.begin(), A.end());
    std::set<Node*> currentNodes = beginNodes;
    std::set<Node*> temp;
    for (char inputA : v) {
        // Transit with character
        temp = currentNodes;
        // Get all epsilon transitions
        if (currentNodes.empty()) {
            currentNodes = eclose(temp);
        } else {
            currentNodes = eclose(currentNodes);
        }
        currentNodes = transit(currentNodes, inputA);
    }
    for (Node* c : eclose(currentNodes)) {
        if (c->isAccepting()) {
            return true;
        }
    }
    return false;
}

DFA ENFA::toDFA() {
    // Create a new DFA
    DFA dfa;
    // Copy over the alphabet
    dfa.setAlphabet(getAlphabet());
    // Create new containers
    std::set<Node*> dfaNodes;
    std::set<Node*> dfaBegin = eclose(beginNodes);
    std::set<Node*> dfaFinalNodes;
    std::set<Transition*> dfaTransitions;
    // Lazy evaluation beginState
    // Create powerset to push to DFA
    std::set<std::set<Node*>> newNodes = {eclose(beginNodes)};
    // Create temporary transitions container
    std::set<TransitionNFA*> tempTransitions;
    // Evaluate
    evaluate(newNodes, tempTransitions);
    eliminateExtra(tempTransitions);
    // Sort state
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
        // Get names of all nodes
        std::vector<std::string> names;
        for (Node* currentNode : currentSet) {
            // Get new node name
            names.push_back(currentNode->getName());
            // Check if accepting
            if (currentNode->isAccepting()) {
                accepting = true;
            }
            // Check if starting
            if (currentSet == dfaBegin) {
                starting = true;
            }
            count++;
        }
        // Combine all names
        count = 0;
        sort(names.begin(), names.end());
        for (const std::string& n : names) {
            newName += n;
            if (count != currentSet.size() - 1) {
                newName += ",";
            }
            count++;
        }
        newName += "}";
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

void ENFA::print() {
    // Make json object
    json j;
    // Set type to NFA
    j["type"] = "ENFA";
    // Set epsilon
    j["eps"] = std::string(1, ENFA::eps);
    // Add alphabet
    // Make temp std::vector
    std::set<std::string> v_Alphabet;
    for (char c : ENFA::alphabet) {
        // Convert char to std::string and insert into std::vector
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

void ENFA::printStats() {
    // Print number of states
    std::cout << "no_of_states=" << nodes.size() << std::endl;
    // Print all epsilon transitions
    std::cout << "no_of_transitions[" << eps << "]=" << epsTransitions.size() << std::endl;
    // Print all the transitions
    for (char c : alphabet) {
        int no_transitions = 0;
        for (Transition* t : transitions) {
            if (t->getInput() == c) {
                no_transitions++;
            }
        }
        std::cout << "no_of_transitions[" << c << "]=" << no_transitions << std::endl;
    }
    // Get degree : number of transitions from this state. degree[i] = number of states with grade i
    // int degree = 0;
    std::map<int, int> degrees;
    // Loop through states
    for (Node* n : nodes) {
        // Get number of transitions
        int node_degree = 0;
        for (Transition* t : transitions) {
            if (t->getBeginNode() == n) {
                // Increase degree
                node_degree++;
            }
        }
        // Insert into map
        degrees[node_degree] += 1;
    }
    // Print all degrees
    for (const auto degree : degrees) {
        std::cout << "degree[" << degree.first << "]=" << degree.second << std::endl;
    }
}

ENFA::~ENFA() {
    for (auto n : nodes) {
        delete n;
    }
    for (auto t : transitions) {
        delete t;
    }
}
