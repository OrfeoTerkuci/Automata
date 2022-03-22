#include "NFA.h"
#include "Node.h"
#include "transition.h"
#include <fstream>
#include <iomanip>
#include "json.hpp"
using namespace std;

using json = nlohmann::json;

NFA::NFA(string filename)
{
    // inlezen uit file
    ifstream input(filename);
    json j;
    input >> j;

    for (json::iterator it = j.begin(); it != j.end(); ++it) {
        // Designate alphabet
        if(it.key() == "alphabet"){
          vector<string> a = it.value();
          for(string s : a){
            alphabet.insert(s[0]);
            }
        }
    }
    
    // Create the nodes
    auto states = j["states"];
    for(auto state : states){
        Node* newState = new Node(state["name"],state["starting"],state["accepting"]);
        nodes.insert(newState);
    }
    // Designate begin and final nodes
    for(Node* n : nodes){
        if(n->isAccepting()){
            finalNodes.insert(n);
        }
        if(n->isStarting()){
            beginNodes.insert(n);
        }
    }
    // Add transitions
    // Create transitions array
    auto ts = j["transitions"];
    Node* beginState;
    Node* endState;
    for(auto t : ts){
        string beginNodeName = t["from"];
        string endNodeName = t["to"];
        string input = t["input"];
        char inputA = input[0];
        for(Node* n : nodes){
            if(n->getName()==beginNodeName){
                beginState = n;
            }
            if(n->getName()==endNodeName){
                endState = n;
            }
        }
        transition* newTransition = new transition(beginState,endState,inputA);
        transitions.insert(newTransition);
    }
}

NFA::NFA() : alphabet({}) , nodes({}) , beginNodes({}) , finalNodes({}) , transitions({}){}

set<char> NFA::getAlphabet() const{
    return NFA::alphabet;
}
set<Node*> NFA::getNodes() const{
    return NFA::nodes;
}
set<Node*> NFA::getFinal() const{
    return NFA::finalNodes;
}
set<Node*> NFA::getBegin() const{
    return NFA::beginNodes;
}
set<transition*> NFA::getTransitions() const{
    return NFA::transitions;
}

void NFA::setAlphabet(set<char>newAlphabet){
    NFA::alphabet = newAlphabet;
}
void NFA::setNodes(set<Node*>newNodes){
    NFA::nodes = newNodes;
}
void NFA::setFinal(set<Node*>newFinalNodes){
    NFA::finalNodes = newFinalNodes;
}
void NFA::setBegin(set<Node*>newBeginNodes){
    NFA::beginNodes = newBeginNodes;
}
void NFA::setTransitions(set<transition*>newTransitions){
    NFA::transitions = newTransitions;
}

set<Node*> NFA::transit(set<Node*> begin , char a){
    set<Node*> c;
    for(transition* t : transitions){
        for(Node* n : begin){
            if(t->getBeginNode() == n && t->getInput() == a){
                c.insert(t->getEndNode());
            }
        } 
    }
    return c;
}

bool NFA::accepts(string A){
    // Split string into chars
    vector<char> v(A.begin(),A.end());
    set<Node*> currentNodes = beginNodes;
    for(char inputA : v){
        currentNodes = transit(currentNodes,inputA);
    }
    for(Node* n : finalNodes){
        for(Node* c : currentNodes){
            if(c == n){
                return true;
            }
        }
    }
    return false;
}

DFA NFA::toDFA(){
    // Create a new DFA
    DFA dfa;
    // Copy over the alphabet
    dfa.setAlphabet(getAlphabet());
    // Create new containers
    set<Node*> dfaNodes;
    set<Node*> dfaBegin;
    set<Node*> dfaFinalNodes;
    set<transition*> dfaTransitions;
    // Copy all the transitions
    transition* newTransition;
    // Lazy evaluation begin
    // Create powerset to push to DFA
    set<set<Node*>>newNodes = {beginNodes};
    // Create counter set -> if all nodes are used
    set<Node*>counter = beginNodes;
    // Begin on beginNodes
    // Create check variable
    bool evaluate = true;
    while(evaluate){
        for(set<Node*>tempNodes : newNodes){
            // Copy all transitions
            for(Node* n : tempNodes){
            for(transition* t : transitions){
                if(t->getBeginNode() == n){
                    newTransition = new transition(t->getBeginNode() , t->getEndNode() , t->getInput());
                    dfaTransitions.insert(newTransition);
                    }
                }
            }
            // Remember old size
            int oldSize = newNodes.size();
            for(char c : alphabet){
                // Transit for character c
                tempNodes = transit(tempNodes,c);
                // Add newly acquired set to newNodes
                newNodes.insert(tempNodes);
            }
            evaluate = !(oldSize == newNodes.size());
        }
    }
    // Create new states
    for(set<Node*> currentSet : newNodes){
        int count = 0;
        // Create new node
        Node* newNode = new Node();
        string newName;
        bool starting = false;
        bool accepting = false;
        // Create combined name
        newName += "{";
        // Add each nodes name
        for(Node* currentNode : currentSet){
            newName += currentNode->getName();
            if(count != currentSet.size() - 1){
                newName += ",";
            }
            else{
                newName += "}";
            }
            // Check if accepting
            if(currentNode->isAccepting()){
                accepting = true;
                }
            if(currentNode->isStarting()){
                starting = true;
                }
            // Check all transitions
            for (transition* t : dfaTransitions){
                // Check if any transition comes from this node
                if(t->getBeginNode() == currentNode){
                    t->setBeginNode(newNode);
                }
                // Check if any transition comes to this node
                if(t->getEndNode() == currentNode){
                    t->setEndNode(newNode);
                }
            }
            count++;
            }
            newNode->setName(newName);
            newNode->setStarting(starting);
            newNode->setAccepting(accepting);
            // Insert the node into the DFA
            dfaNodes.insert(newNode);
            // Check if accepting
            if(newNode->isAccepting()){
                dfaFinalNodes.insert(newNode);
            }
            if(newNode->isStarting()){
                dfaBegin.insert(newNode);
            }
        }
        // Set all the containters to the dfa
        dfa.setNodes(dfaNodes);
        dfa.setBegin(dfaBegin);
        dfa.setFinal(dfaFinalNodes);
        dfa.setTransitions(dfaTransitions);
        return dfa;
    }


void NFA::print(){
   // Make json object
   json j;
   // Set type to NFA
   j["type"] = "NFA";
   // Add alphabet
   // Make temp vector
   set<string> v_Alphabet;
   for (char c : NFA::alphabet){
       // Convert char to string and insert into vector
       v_Alphabet.insert(string(1,c));
   }
   j["alphabet"] = v_Alphabet;
   // Add states
   json states_array = json::array();
   for(auto s : nodes){
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
   for (auto t : transitions){
       // Make new object and add each element of transition
       json newObject = json::object();
       newObject["from"] = t->getBeginNode()->getName();
       newObject["to"] = t->getEndNode()->getName();
       newObject["input"] = string(1,t->getInput());
       transitions_array.push_back(newObject);
   }
   j["transitions"] = transitions_array;
   // Print to screen
   cout<< setw(4) << j << endl;
}

NFA::~NFA()
{

}