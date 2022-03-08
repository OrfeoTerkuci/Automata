#include "Node.h"
#include "DFA.h"
#include "transition.h"
#include <iostream>
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
    Node* currentNode = beginNodes[0];
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
    /*
    // tonen op het scherm
    cout << setw(4) << j << endl;

    // manueel aanmaken
    json j2;
    j2["type"] = "DFA";
    j2["alphabet"] = {"0", "1"};
    cout << setw(4) << j2 << endl;
    */
   // Make json object
   json j;
   // Set type to DFA
   j["type"] = "DFA";
   // Add alphabet
   // Make temp vector
   vector<string> v_Alphabet;
   for (char c : DFA::alphabet){
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

DFA::~DFA()
{

}
