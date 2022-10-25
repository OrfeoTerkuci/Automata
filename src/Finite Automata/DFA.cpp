#include "DFA.h"
#include "Node.h"
#include "transition.h"
#include "transitionNFA.h"
#include <fstream>
#include <iomanip>
#include "../json.hpp"

using json = nlohmann::json;

DFA::DFA(const std::string& filename)
{
    //* Read from file
    std::ifstream input(filename);
    json j;
    input >> j;

    for (json::iterator it = j.begin(); it != j.end(); ++it) {
        // Designate alphabet
        if(it.key() == "alphabet"){
          std::vector<std::string> a = it.value();
          for(std::string s : a){
            alphabet.insert(s[0]);
            }
        }
    }

    //* Create the nodes
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
    //* Add transitions
    // Create transitions array
    auto ts = j["transitions"];
    Node* beginState;
    Node* endState;
    for(auto t : ts){
        std::string beginNodeName = t["from"];
        std::string endNodeName = t["to"];
        std::string t_input = t["input"];
        char inputA = t_input[0];
        for(Node* n : nodes){
            if(n->getName()==beginNodeName){
                beginState = n;
            }
            if(n->getName()==endNodeName){
                endState = n;
            }
        }
        auto* newTransition = new transition(beginState,endState,inputA);
        transitions.insert(newTransition);
    }
    //* Create beginning TFA table
    createTable();
}

DFA::DFA(DFA &dfa1, DFA &dfa2 , bool intersect) {
    //* Copy alphabet
    for( char c : dfa1.getAlphabet() ){
        alphabet.insert(c);
    }
    for( char c : dfa2.getAlphabet() ){
        alphabet.insert(c);
    }
    //* Set begin state as the pair of both begin states
    beginNodes.insert( *dfa1.getBegin().begin() );
    beginNodes.insert( *dfa2.getBegin().begin() );
    //* Copy all the nodes
    for( Node* n : dfa1.getNodes() ){
        nodes.insert(n);
    }
    for( Node* n : dfa2.getNodes() ){
        nodes.insert(n);
    }
    //* Copy all the transitions
    for( transition* t : dfa1.getTransitions() ){
        transitions.insert(t);
    }
    for( transition* t : dfa2.getTransitions() ){
        transitions.insert(t);
    }
    //* Begin lazy evaluation
    std::set<transitionNFA*> tempTransitions;
    std::set<std::set<Node*>> newNodes = {beginNodes};
    evaluate(newNodes , tempTransitions);
    eliminateExtra(tempTransitions);
    //* Reset nodes
    beginNodes.clear();
    nodes.clear();
    transitions.clear();
    //* Create new states
    for(const std::set<Node*>& currentSet : newNodes){
        int count = 0;
        //* Create new node
        Node* newNode = new Node();
        std::string newName;
        // Both states must be starting
        bool starting = true;
        // Intersection: initialize true
        bool accepting = intersect;
        
        //* Create combined name
        newName += ")";
        //* Add each node's name
        for(Node* currentNode : currentSet){
            // Get new node name
            newName += currentNode->getName();
            if(count != currentSet.size() - 1){
                newName += ",";
                }
            else{
                newName += "(";
                }
            if(!currentNode->isStarting()){
                starting = false;
            }
            //* Check if accepting
            // Union: one accepting = accepting
            if(!intersect && currentNode->isAccepting()){
                accepting = true;
            }
            // If intersection: one not accepting = not accepting
            else if(intersect && !currentNode->isAccepting()){
                accepting = false;
            }
            count++;
            }
        //* Check all transitions
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
        std::reverse(newName.begin() , newName.end());
        newNode->setName(newName);
        newNode->setStarting(starting);
        newNode->setAccepting(accepting);
        //* Insert the node into the DFA
        nodes.insert(newNode);
        // Check if starting
        if(newNode->isStarting()){
            beginNodes.insert(newNode);
            }
        if(newNode->isAccepting()){
            finalNodes.insert(newNode);
            }
        
    }
    //* Convert all transitions to dfa transitions
    transition* nt;
    for(transitionNFA* t : tempTransitions){
        nt = new transition(*t->getBeginNodes().begin() , *t->getEndNodes().begin() , t->getInput());
        transitions.insert(nt);
        delete t;
    }
    //* Create beginning TFA table
    createTable();
}

DFA::DFA() : alphabet({}) , nodes({}) , beginNodes({}) , finalNodes({}) , transitions({}) , table({}){}

std::set<char> DFA::getAlphabet() const{
    return DFA::alphabet;
}
std::set<Node*> DFA::getNodes() const{
    return DFA::nodes;
}

__attribute__((unused)) std::set<Node*> DFA::getFinal() const{
    return DFA::finalNodes;
}
std::set<Node*> DFA::getBegin() const{
    return DFA::beginNodes;
}

std::set<transition*> DFA::getTransitions() const{
    return DFA::transitions;
}
void DFA::setAlphabet(std::set<char>newAlphabet){
    DFA::alphabet = newAlphabet;
}
void DFA::setNodes(std::set<Node*>newNodes){
    DFA::nodes = newNodes;
}
void DFA::setFinal(std::set<Node*>newFinalNodes){
    DFA::finalNodes = newFinalNodes;
}
void DFA::setBegin(std::set<Node*>newBeginNodes){
    DFA::beginNodes = newBeginNodes;
}

void DFA::setTransitions(std::set<transition*>newTransitions){
    DFA::transitions = newTransitions;
}

void DFA::evaluate(std::set<std::set<Node*>> &newNodes , std::set<transitionNFA*> &tempTransitions) {
    bool evaluate = true;
    // Create containers
    transitionNFA* newTransition;
    while(evaluate){
        // Remember old size
        int oldSize = static_cast<int>(newNodes.size());
        // Loop through all node pairs
        for(const std::set<Node*>& currentNodes : newNodes){
            // Remember beginning
            std::set<Node*> oldTemp;
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

void DFA::eliminateExtra(std::set<transitionNFA*> &currentSet){
    for (auto it1 = currentSet.begin(); it1 != currentSet.end(); it1++){
        for (auto it2 = currentSet.begin(); it2 != currentSet.end(); it2++){
            if(it1 == it2){
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
            if( b1 == b2 && e1 == e2 && c1 == c2 ){
                delete t2;
                it2 = currentSet.erase(it2);
            }
        }
    }
}

void DFA::eliminateExtra(std::set<transition*>&trans){
    for (auto it1 = trans.begin(); it1 != trans.end(); it1++){
        for (auto it2 = trans.begin(); it2 != trans.end(); it2++){
            if(it1 == it2){
                continue;
            }
            // Check for duplicate
            auto t1 = *it1;
            auto t2 = *it2;
            Node* b1 = t1->getBeginNode();
            Node* e1 = t1->getEndNode();
            Node* b2 = t2->getBeginNode();
            Node* e2 = t2->getEndNode();
            char c1 = t1->getInput();
            char c2 = t2->getInput();
            if( b1 == b2 && e1 == e2 && c1 == c2 ){
                delete t2;
                it2 = trans.erase(it2);
            }
        }
    }
}

Node* DFA::transit(Node* begin , char a){
    if(alphabet.find(a) == alphabet.end()){
        return nullptr;
    }
    for(transition* t : transitions){
        if(t->getBeginNode() == begin && t->getInput() == a){
            return t->getEndNode();
        }
    }
    return begin;
}

bool DFA::accepts(std::string A){
    Node* currentNode = *beginNodes.begin();
    for(char inputA : A){
        currentNode = transit(currentNode,inputA);
        if(currentNode == nullptr){
            return false;
        }
    }
    return currentNode->isAccepting();
}

void DFA::print(){
   // Make json object
   json j;
   // Set type to DFA
   j["type"] = "DFA";
   // Add alphabet
   std::set<std::string> v_Alphabet;
   for (char c : DFA::alphabet){
       // Convert char to string and insert into vector
       v_Alphabet.insert(std::string(1,c));
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
       newObject["input"] = std::string(1,t->getInput());
       transitions_array.push_back(newObject);
   }
   j["transitions"] = transitions_array;
   // Print to screen
   std::cout<< std::setw(4) << j << std::endl;
}

void DFA::createTable(){
    // Create empty sets
    std::set<std::string> newSet;
    std::set<Node*> newMarked;
    // Crossed?
    bool diff;
    // Create all the pairs
    for(Node* n : nodes){
        for(Node* m : nodes){
            // Get pair's names
            newSet.insert(n->getName());
            newSet.insert(m->getName());
            // Get pair's states
            newMarked.insert(n);
            newMarked.insert(m);
            if(newSet.size() == 2){
                diff = ( ( n->isAccepting() && !m->isAccepting() ) || ( !n->isAccepting() && m->isAccepting() ) );
                table[newSet] = diff;
                if(diff){
                    markedPairs.insert( newMarked );
                }
            }
            // Reset sets
            newSet = {};
            newMarked = {};
        }
    }
}

DFA DFA::minimize(){
    // Create new empty DFA and containers for it
    DFA newDFA = DFA();
    std::set<Node*> newDFA_nodes;
    std::set<transition*> newDFA_transitions;
    std::set<Node*> newDFA_final;
    std::set<Node*> newDFA_begin;
    // Set alphabet
    newDFA.setAlphabet(alphabet);
    // Copy the transitions
    for(auto t : transitions){
        newDFA_transitions.insert(new transition(t));
    }
    // Execute TFA
    fillTable();
    // Get all equivalent sets of states
    std::set<std::set<Node*>> newStates;
    std::set<Node*> newSet;
    // Create new node and components for it
    Node* newNode;
    std::string newName;
    bool accepting = false;
    bool starting = false;
    // Get all the unmarked pairs
    std::set<std::set<Node*>> unmarked;
    for(const auto& p : table){
        if(!p.second){
            for(auto n : nodes){
                if(p.first.find(n->getName()) != p.first.end()){
                    newSet.insert(n);
                }
            }
            unmarked.insert(newSet);
            newSet = {};
        }
    }
    // Merge the pairs with common elements
    for(const auto &p : unmarked){
        newSet = p;
        // Check if there is a common element with another set
        for(const auto& q : unmarked){
            for(auto n : q){
                if(newSet.find(n) != newSet.end()){
                    newSet.insert(q.begin() , q.end());
                    break;
                }
            }
        }
        newStates.insert(newSet);
    }
    // Reset container
    unmarked = {};
    // Get all the marked pairs
    std::set<Node*> marked = nodes;
    std::vector<std::string> names;
    for(const auto& s : newStates){
        newNode = new Node();
        for(Node* n : s){
            marked.erase(n);
            names.push_back(n->getName());
            // Check if starting
            if(n->isStarting()){
                starting = true;
            }
            if(n->isAccepting()){
                accepting = true;
            }
            // Relink the transitions
            for(auto t : newDFA_transitions){
                if(t->getBeginNode() == n){
                    t->setBeginNode(newNode);
                }
                if(t->getEndNode() == n){
                    t->setEndNode(newNode);
                }
            }
        }
        std::sort(names.begin() , names.end());
        // Merge the name : {state1,state2}
        newName = "{";
        for(const std::string &name : names){
            newName += name;
            if(name!= *names.rbegin()){
                newName += ", ";
            }
            else{
                newName += "}";
            }
        }
        // Create new state
        newNode->setName(newName);
        newNode->setStarting(starting);
        newNode->setAccepting(accepting);
        // Insert state into the states vector
        newDFA_nodes.insert(newNode);
        // Reset components
        starting = false;
        accepting = false;
        // Reset container
        names = {};
    }
    // Remove extra transitions
    eliminateExtra(newDFA_transitions);
    // Create new nodes from the marked ones
    for(auto n : marked){
        // Create new node
        newNode = new Node("{" + n->getName() + "}" , n->isStarting() , n->isAccepting());
        newDFA_nodes.insert(newNode);
        // Relink transitions
        for(auto t : newDFA_transitions){
            if(t->getBeginNode() == n){
                t->setBeginNode(newNode);
            }
            if(t->getEndNode() == n){
                t->setEndNode(newNode);
            }
        }
    }
    for(auto n : newDFA_nodes){
        if(n->isStarting()){
            newDFA_begin.insert(n);
        }
        if(n->isAccepting()){
            newDFA_final.insert(n);
        }
    }
    newDFA.setNodes(newDFA_nodes);
    newDFA.setTransitions(newDFA_transitions);
    newDFA.setBegin(newDFA_begin);
    newDFA.setFinal(newDFA_final);
    return newDFA;
}

std::set<std::set<Node*>> DFA::findTransition(std::set<Node*> &beginNodesVector , char c){
    // Find a pair of nodes that transits to the given set with char c
    std::set<Node*> tempNode = {};
    std::set<Node*> newSet;
    std::set< std::set<Node*> > newNode;
    for(auto n : nodes){
        for(auto m : nodes){
            tempNode.insert(transit(n , c));
            tempNode.insert(transit(m , c));
            if(tempNode == beginNodesVector){
                newSet.insert(n);
                newSet.insert(m);
                newNode.insert(newSet);
            }
            // Reset tempNode
            tempNode = {};
            newSet = {};
        }
    }
    return newNode;
}

void DFA::fillTable(){
    bool evaluate = true;
    std::set< std::set<Node*> > newPair;
    std::set<std::string> newNames;
    std::vector<std::string> newMarked;
    std::set<Node*> tempPair;
    Node* newB;
    Node* newE;
    int oldSize;
    while(evaluate){
        // Remember old size
        oldSize = static_cast<int>(markedPairs.size());
        // Loop through marked pairs
        for(auto p : markedPairs){
            // Loop through alphabet
            for(char c : alphabet){
                // Find a transition to this marked pair
                newPair = findTransition(p , c);
                // Check if transition is valid
                for(const auto &newP : newPair){
                    // Get first and second node of the pair
                    newB = *newP.begin();
                    newE = *newP.rbegin();
                    // Get node names
                    newMarked.push_back(newB->getName());
                    newMarked.push_back(newE->getName());
                    std::sort(newMarked.begin() , newMarked.end());
                    newNames.insert(newMarked.begin() , newMarked.end());
                    // Insert new pair into markedPairs set
                    markedPairs.insert(newP);
                    // Insert new pair names into table
                    table[newNames] = true;
                    // Reset containers
                    newMarked = {};
                    newNames = {};
                }
                // Reset pairs
                newPair = {};
                tempPair = {};
            }
        }
        // Evaluate?
        evaluate = (oldSize != markedPairs.size());
    } 
}

void DFA::printTable(){
   std::vector<std::string> tempNodes;
   for(auto n : nodes){
       tempNodes.push_back( n->getName() );
   }
   std::sort( tempNodes.begin() , tempNodes.end() );
   int count = 1;
   // Loop from 2nd element to last element
   for(int i = 1; i < tempNodes.size(); i++){
       // Print row name
       std::cout << tempNodes[i] << '\t';
       // Loop until count
       for(int j = 0; j < count; j++){
           if( table[ { tempNodes[i] , tempNodes[j] }] ){
               std::cout << 'X' << '\t';
           }
           else{
               std::cout << '-' << '\t';
           }
       }
       std::cout << std::endl;
       count++;
   }
   std::cout << '\t';
   for(int i = 0; i < tempNodes.size() - 1; i++){
       std::cout << tempNodes[i];
       if(i != tempNodes.size() - 1){
           std::cout << '\t';
       }
   }
   std::cout << std::endl;
}

bool DFA::operator==(DFA &dfa2){
    // Save original nodes and transitions
    std::set<Node*> originalNodes = nodes;
    std::set<transition*> originalTransitions = transitions;
    // Get all the nodes from both DFAs
    for(auto n : dfa2.getNodes()){
        nodes.insert(n);
    }
    for(auto t : dfa2.getTransitions()){
        transitions.insert(t);
    }
    // Execute TFA
    createTable();
    fillTable();
    // Print filled table
    printTable();
    // Check whether the pair of begin nodes has been filled
    Node* b1 = *getBegin().begin();
    Node* b2 = *dfa2.getBegin().begin();
    // Reset the nodes and transitions
    nodes = originalNodes;
    transitions = originalTransitions;

    return !table[ { b1->getName() , b2->getName() } ];
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
