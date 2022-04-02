#include "DFA.h"
#include "Node.h"
#include "transition.h"
#include "transitionNFA.h"
#include <fstream>
#include <iomanip>
#include "json.hpp"
using namespace std;

using json = nlohmann::json;

DFA::DFA(string filename)
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

DFA::DFA(DFA dfa1, DFA dfa2 , bool intersect) {
    // Copy alphabet
    for( char c : dfa1.getAlphabet() ){
        alphabet.insert(c);
    }
    for( char c : dfa2.getAlphabet() ){
        alphabet.insert(c);
    }
    // Set begin state as the pair of both begin states
    beginNodes.insert( *dfa1.getBegin().begin() );
    beginNodes.insert( *dfa2.getBegin().begin() );
    // Copy all the nodes
    for( Node* n : dfa1.getNodes() ){
        nodes.insert(n);
    }
    for( Node* n : dfa2.getNodes() ){
        nodes.insert(n);
    }
    // Copy all the transitions
    for( transition* t : dfa1.getTransitions() ){
        transitions.insert(t);
    }
    for( transition* t : dfa2.getTransitions() ){
        transitions.insert(t);
    }
    // Begin lazy evaluation
    set<transitionNFA*> tempTransitions;
    set<set<Node*>> newNodes = {beginNodes};
    evaluate(newNodes , tempTransitions);
    eliminateExtra(tempTransitions);
    // Reset nodes
    beginNodes.clear();
    nodes.clear();
    transitions.clear();
    // Create new states
    for(set<Node*> currentSet : newNodes){
        int count = 0;
        // Create new node
        Node* newNode = new Node();
        string newName;
        // Both states must be starting
        bool starting = true;
        // Intersection: initialize true
        bool accepting = intersect;
        
        // Create combined name
        newName += ")";
        // Add each nodes name
        //vector<string> names;
        for(Node* currentNode : currentSet){
            // Get new node name
            newName += currentNode->getName();
            if(count != currentSet.size() - 1){
                newName += ",";
                }
            else{
                newName += "(";
                }
            // If union: one accepting = accepting
            if(!currentNode->isStarting()){
                starting = false;
            }
            // Check if accepting
            // If union: one accepting = accepting
            if(!intersect && currentNode->isAccepting()){
                accepting = true;
            }
            // If intersection: one not accepting = not accepting
            else if(intersect && !currentNode->isAccepting()){
                accepting = false;
            }
            count++;
            }
        // Check all transitions
        for (transitionNFA* t : tempTransitions){
            // Check if transition begins at this set
            if(t->getBeginNodes() == currentSet){
                t->setBeginNodes({newNode});
                }
            // Check if transition ends at this set
            if(t->getEndNodes() == currentSet){
                t->setEndNodes({newNode});
                }
            }
        reverse(newName.begin() , newName.end());
        newNode->setName(newName);
        newNode->setStarting(starting);
        newNode->setAccepting(accepting);
        // Insert the node into the DFA
        nodes.insert(newNode);
        // Check if starting
        if(newNode->isStarting()){
            beginNodes.insert(newNode);
            }
        if(newNode->isAccepting()){
            finalNodes.insert(newNode);
            }
        
    }
    // Convert all transitions to dfa transitions
    transition* nt;
    for(transitionNFA* t : tempTransitions){
        nt = new transition(*t->getBeginNodes().begin() , *t->getEndNodes().begin() , t->getInput());
        transitions.insert(nt);
    }
}

DFA::DFA() : alphabet({}) , nodes({}) , beginNodes({}) , finalNodes({}) , transitions({}){}

set<char> DFA::getAlphabet() const{
    return DFA::alphabet;
}
set<Node*> DFA::getNodes() const{
    return DFA::nodes;
}
set<Node*> DFA::getFinal() const{
    return DFA::finalNodes;
}
set<Node*> DFA::getBegin() const{
    return DFA::beginNodes;
}

set<transition*> DFA::getTransitions() const{
    return DFA::transitions;
}
void DFA::setAlphabet(set<char>newAlphabet){
    DFA::alphabet = newAlphabet;
}
void DFA::setNodes(set<Node*>newNodes){
    DFA::nodes = newNodes;
}
void DFA::setFinal(set<Node*>newFinalNodes){
    DFA::finalNodes = newFinalNodes;
}
void DFA::setBegin(set<Node*>newBeginNodes){
    DFA::beginNodes = newBeginNodes;
}

void DFA::setTransitions(set<transition*>newTransitions){
    DFA::transitions = newTransitions;
}

void DFA::evaluate(set<set<Node*>> &newNodes , set<transitionNFA*> &tempTransitions) {
    bool evaluate = true;
    // Create containers
    transitionNFA* newTransition;
    while(evaluate){
        // Remember old size
        int oldSize = static_cast<int>(newNodes.size());
        // Loop through all node pairs
        for(set<Node*> currentNodes : newNodes){
            // Remember beginning
            set<Node*> oldTemp;
            // Loop through the alphabet
            for( char c: alphabet){
                // Loop through both nodes
                for(Node* tempNode : currentNodes){
                    // Add end node of transition
                    oldTemp.insert( transit(tempNode , c ) );
                }
                // Add newly acquired pair
                newNodes.insert(oldTemp);
                // Add transitions
                if(!oldTemp.empty()){
                    // Add used transitions
                    newTransition = new transitionNFA();
                    newTransition->setBeginNodes(currentNodes);
                    // Add end nodes to transition
                    newTransition->setEndNodes(oldTemp);
                    newTransition->setInput(c);
                    // Add transition to container
                    tempTransitions.insert(newTransition);
                }
                // Reset the new node
                oldTemp.clear();
            }
        }
        evaluate = oldSize != newNodes.size();
    }
}

void DFA::eliminateExtra(set<transitionNFA*> &currentSet){
    for (auto it1 = currentSet.begin(); it1 != currentSet.end(); it1++){
        for (auto it2 = currentSet.begin(); it2 != currentSet.end(); it2++){
            if(it1 == it2){
                continue;
            }
            // Check for duplicate
            auto t1 = *it1;
            auto t2 = *it2;
            set<Node*> b1 = t1->getBeginNodes();
            set<Node*> e1 = t1->getEndNodes();
            set<Node*> b2 = t2->getBeginNodes();
            set<Node*> e2 = t2->getEndNodes();
            char c1 = t1->getInput();
            char c2 = t2->getInput();
            if( b1 == b2 && e1 == e2 && c1 == c2 ){
                delete t2;
                it2 = currentSet.erase(it2);
            }
        }
    }
}

Node* DFA::transit(Node* begin , char a){
    for(transition* t : transitions){
        if(t->getBeginNode() == begin && t->getInput() == a){
            return t->getEndNode();
        }
    }
    return begin;
}

bool DFA::accepts(string A){
    // Split string into chars
    vector<char> v(A.begin(),A.end());
    Node* currentNode = *beginNodes.begin();
    for(char inputA : v){
        currentNode = transit(currentNode,inputA);
    }
    for(Node* n : finalNodes){
        if(n == currentNode){
            return true;
        }
    }
    return false;
}

void DFA::print(){
   // Make json object
   json j;
   // Set type to DFA
   j["type"] = "DFA";
   // Add alphabet
   // Make temp set
   set<string> v_Alphabet;
   for (char c : DFA::alphabet){
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

DFA::~DFA()
{
    for(auto n : nodes){
        delete n;
    }
    for(auto t : transitions){
        delete t;
    }
}
