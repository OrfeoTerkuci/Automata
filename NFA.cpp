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
            alphabet.push_back(s[0]);
            }
        }
    }
    
    // Create the nodes
    auto states = j["states"];
    for(auto state : states){
        Node* newState = new Node(state["name"],state["starting"],state["accepting"]);
        nodes.push_back(newState);
    }
    // Designate begin and final nodes
    for(Node* n : nodes){
        if(n->isAccepting()){
            finalNodes.push_back(n);
        }
        if(n->isStarting()){
            beginNodes.push_back(n);
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
        transitions.push_back(newTransition);
    }
}

vector<Node*> NFA::transit(vector<Node*> begin , char a){
    vector<Node*> c;
    for(transition* t : transitions){
        for(Node* n : begin){
            if(t->getBeginNode() == n && t->getInput() == a){
                c.push_back(t->getEndNode());
            }
        } 
    }
    return c;
}

NFA::NFA() : alphabet({}) , nodes({}) , beginNodes({}) , finalNodes({}) , transitions({}){}

vector<char> NFA::getAlphabet() const{
    return NFA::alphabet;
}
vector<Node*> NFA::getNodes() const{
    return NFA::nodes;
}
vector<Node*> NFA::getFinal() const{
    return NFA::finalNodes;
}
vector<Node*> NFA::getBegin() const{
    return NFA::beginNodes;
}
vector<transition*> NFA::getTransitions() const{
    return NFA::transitions;
}

void NFA::setAlphabet(vector<char>newAlphabet){
    NFA::alphabet = newAlphabet;
}
void NFA::setNodes(vector<Node*>newNodes){
    NFA::nodes = newNodes;
}
void NFA::setFinal(vector<Node*>newFinalNodes){
    NFA::finalNodes = newFinalNodes;
}
void NFA::setBegin(vector<Node*>newBeginNodes){
    NFA::beginNodes = newBeginNodes;
}
void NFA::setTransitions(vector<transition*>newTransitions){
    NFA::transitions = newTransitions;
}

bool NFA::accepts(string A){
    // Split string into chars
    vector<char> v(A.begin(),A.end());
    vector<Node*> currentNodes = beginNodes;
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
    DFA dfa;
    dfa.setAlphabet(getAlphabet());
    dfa.setBegin(getBegin());
    // Lazy evaluation begin
    vector<Node*>newNodes;
    vector<Node*>tempNodes = beginNodes;
    vector<Node*>counter = beginNodes;
    // Begin on beginNodes
    string newName;
    bool starting;
    bool accepting;
    while(counter.size() < nodes.size()){
        for(char c : alphabet){
            tempNodes = transit(tempNodes,c);
            for(Node* n : transit(tempNodes,c)){
                // Add newly acquired nodes
                if(find(tempNodes.begin(),tempNodes.end(),n) == tempNodes.end()){
                    counter.push_back(n);
                }
            }
        }

        for(Node* n : tempNodes){
            if(n == tempNodes.front()){
                newName += "{";
            }
            newName += n->getName();
            if(n->isStarting()){
                starting = true;
            }
            if(n->isAccepting()){
                accepting = true;
            }
            if(n != tempNodes.back()){
                newName += ",";
            }
            else{
                newName += "}";
            }
        }
        Node* newNode;
        newNode = new Node(newName,starting,accepting);
        newNodes.push_back(newNode);
    }
    // For each new state , repeat
    dfa.setNodes(newNodes);
    return dfa;
}


void NFA::print(){
   // Make json object
   json j;
   // Set type to NFA
   j["type"] = "NFA";
   // Add alphabet
   // Make temp vector
   vector<string> v_Alphabet;
   for (char c : NFA::alphabet){
       // Convert char to string and insert into vector
       v_Alphabet.push_back(string(1,c));
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