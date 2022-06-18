#include "RE.h"
#include "Node.h"
#include "transition.h"
#include "transitionNFA.h"
#include <algorithm>
#include <math.h>
#include <string>
#include <vector>

RE::RE(const std::string &regex, char eps) : regex(regex), eps(eps) {}

RE::RE(RE* refRE) : regex(refRE->getRegex()) , eps(refRE->getEps()) {}

RE::RE() : regex(" ") , eps(' ') {}

const std::string &RE::getRegex() const {
    return regex;
}

void RE::setRegex(const std::string &regex) {
    RE::regex = regex;
}

char RE::getEps() const {
    return eps;
}

void RE::setEps(char eps) {
    RE::eps = eps;
}

ENFA* RE::createEpsilon(std::string beginName , std::string endName) {
    // Create states
    Node* begin = new Node(beginName , true , false);
    Node* end = new Node(endName , false , true);
    // Create epsilon transition
    auto * newTrans = new transition(begin , end , eps);
    // Create new ENFA
    ENFA* newENFA = new ENFA();
    // Set all parameters
    newENFA->setEps(eps);
    newENFA->setAlphabet({});
    newENFA->setNodes({begin , end});
    newENFA->setBegin({begin});
    newENFA->setFinal({end});
    newENFA->setTransitions({newTrans});
    newENFA->setEpsTransitions({newTrans});
    return newENFA;
}

ENFA* RE::createEmpty(std::string beginName , std::string endName) {
    // Create states
    Node* begin = new Node(beginName , true , false);
    Node* end = new Node(endName , false , true);
    // Create new ENFA
    ENFA* newENFA = new ENFA();
    // Set all parameters
    newENFA->setEps(eps);
    newENFA->setAlphabet({});
    newENFA->setNodes({begin , end});
    newENFA->setBegin({begin});
    newENFA->setFinal({end});
    newENFA->setTransitions({});
    return newENFA;
}

ENFA* RE::createSingleChar(std::string beginName , std::string endName , char a) {
    // Create states
    Node* begin = new Node(beginName , true , false);
    Node* end = new Node(endName , false , true);
    // Create epsilon transition
    auto * newTrans = new transition(begin , end , a);
    // Create new ENFA
    ENFA* newENFA = new ENFA();
    // Set all parameters
    newENFA->setEps(eps);
    newENFA->setAlphabet({a});
    newENFA->setNodes({begin , end});
    newENFA->setBegin({begin});
    newENFA->setFinal({end});
    newENFA->setTransitions({newTrans});
    return newENFA;
}

ENFA* RE::createPlus(std::vector<ENFA*> &ref , int &count) {

    std::string beginName = std::to_string(count);
    std::string endName = std::to_string(count + 1);
    count += 2;
    // Create begin and end states
    Node* begin = new Node(beginName , true , false);
    Node* end = new Node(endName , false , true);
    // Create containers
    std::set<char> alphabet;
    std::set<Node*> nodes = {begin , end};
    std::set<transition*> transitions;
    std::set<transition*> eps_transitions;
    // Create new ENFA
    ENFA* newENFA = new ENFA();
    ENFA* e;
    ENFA* f;
    std::vector<ENFA*> temp;

    if(ref.size() == 1){
        return ref[0];
    }
    // Link the new begin and end node to both ENFA's
    // Get current and next ENFA in the vector
    // If more than 2 : recursive call until 2 left
    while(ref.size() > 2){
        e = ref[0];
        f = ref[1];
        temp = {e,f};
        newENFA = createPlus(temp , count);
        ref.erase(ref.begin());
        *ref.begin() = newENFA;
    }
    e = ref[0];
    f = ref[1];
    // Create new transition from begin node to begin node of the ENFA
    auto* newTransE_begin = new transition(begin , *e->getBegin().begin(), eps);
    newTransE_begin->getEndNode()->setStarting(false);
    transitions.insert(newTransE_begin);

    // Create new transition from end node of the ENFA to the end node
    auto* newTransE_end = new transition(*e->getFinal().begin() , end, eps);
    newTransE_end->getBeginNode()->setAccepting(false);
    transitions.insert(newTransE_end);

    // Create new transition from begin node to begin node of the ENFA
    auto* newTransF_begin = new transition(begin , *f->getBegin().begin(), eps);
    newTransF_begin->getEndNode()->setStarting(false);
    transitions.insert(newTransF_begin);

    // Create new transition from end node of the ENFA to the end node
    auto* newTransF_end = new transition(*f->getFinal().begin() , end, eps);
    newTransF_end->getBeginNode()->setAccepting(false);
    transitions.insert(newTransF_end);

    // Insert character into alphabet
    for(auto c : e->getAlphabet()){
        alphabet.insert(c);
    }
    // Insert all the nodes
    for(auto n : e->getNodes()){
        nodes.insert(n);
    }
    // Insert all the transitions
    for(auto t : e->getTransitions()){
        transitions.insert(t);
    }
    // Insert all the epsilon transitions
    for(auto t : e->getEpsTransitions()){
        eps_transitions.insert(t);
    }
    // Insert character into alphabet
    for(auto c : f->getAlphabet()){
        alphabet.insert(c);
    }
    // Insert all the nodes
    for(auto n : f->getNodes()){
        nodes.insert(n);
    }
    // Insert all the transitions
    for(auto t : f->getTransitions()){
        transitions.insert(t);
    }
    // Insert all the epsilon transitions
    for(auto t : f->getEpsTransitions()){
        eps_transitions.insert(t);
    }
    
    // Set all parameters
    newENFA->setEps(eps);
    newENFA->setAlphabet(alphabet);
    newENFA->setNodes(nodes);
    newENFA->setBegin({begin});
    newENFA->setFinal({end});   
    newENFA->setTransitions(transitions);
    // Get epsilon transitions
    for(auto t : transitions){
        if(t->getInput() == newENFA->getEps()){
            eps_transitions.insert(t);
        }
    }
    newENFA->setEpsTransitions(eps_transitions);
    return newENFA;
}

ENFA* RE::createConcatenation(ENFA &R , ENFA &S) {
    // Create containers
    std::set<char> alphabet;
    std::set<Node*> nodes;
    std::set<transition*> transitions;
    std::set<transition*> eps_transitions;
    // Create new ENFA
    ENFA* newENFA = new ENFA();
    // Create new transition
    transition* newTrans = new transition(*R.getFinal().begin() , *S.getBegin().begin() , eps);
    transitions.insert(newTrans);
    // Set all parameters
    newENFA->setEps(eps);
    newENFA->setBegin(R.getBegin());
    newENFA->setFinal(S.getFinal());
    newTrans->getBeginNode()->setAccepting(false);
    newTrans->getEndNode()->setStarting(false);
    // Copy alphabet
    for(char c : R.getAlphabet()){
        alphabet.insert(c);
    }
    for(char c : S.getAlphabet()){
        alphabet.insert(c);
    }
    newENFA->setAlphabet(alphabet);
    // Copy nodes
    for(Node* n : R.getNodes()){
        nodes.insert(n);
    }
    for(Node* n : S.getNodes()){
        nodes.insert(n);
    }
    newENFA->setNodes(nodes);
    // Copy transitions
    for(transition* t : R.getTransitions()){
        transitions.insert(t);
    }
    for(transition* t : S.getTransitions()){
        transitions.insert(t);
    }
    newENFA->setTransitions(transitions);
    for(transition* t : transitions){
        if(t->getInput() == newENFA->getEps()){
            eps_transitions.insert(t);
        }
    }
    newENFA->setEpsTransitions(eps_transitions);
    return newENFA;
}

ENFA* RE::createConcatenation(std::vector<ENFA*> ref){
    
    // Create containers
    std::set<char> alphabet;
    std::set<Node*> nodes;
    std::set<transition*> transitions;
    std::set<transition*> eps_transitions;
    // Create new ENFA
    ENFA* newENFA = new ENFA();
    for(int i = 0; i < ref.size(); i++){
        // Get the current ENFA
        ENFA* current = ref[i];
        ENFA* next;
        // Make new empty transition
        transition* newTrans;
        // Get alphabet
        for(char c : current->getAlphabet()){
            alphabet.insert(c);
        }
        // If not the last enfa
        if(i != ref.size() - 1){
            // Get the next enfa
            next = ref[i+1];
            // Create new epsilon transition
            newTrans = new transition(*current->getFinal().begin() , *next->getBegin().begin() , eps);
            newTrans->getBeginNode()->setAccepting(false);
            newTrans->getEndNode()->setStarting(false);
            transitions.insert(newTrans);
            eps_transitions.insert(newTrans);
        }
        // Copy nodes
        for(auto n : current->getNodes()){
            nodes.insert(n);
        }
        // Copy transitions
        for(auto t : current->getTransitions()){
            transitions.insert(t);
        }
        for(auto t : current->getEpsTransitions()){
            eps_transitions.insert(t);
        }
    }
    // Set all parameters
    newENFA->setEps(eps);
    newENFA->setAlphabet(alphabet);
    newENFA->setNodes(nodes);
    newENFA->setBegin(ref.front()->getBegin());
    newENFA->setFinal(ref.back()->getFinal());
    newENFA->setTransitions(transitions);
    newENFA->setEpsTransitions(eps_transitions);
    return newENFA;
}

ENFA* RE::createStar(std::string beginName , std::string endName , ENFA &R) {

    // Create new containers and copy elements from ref ENFA
    std::set<char> alphabet = R.getAlphabet();
    std::set<Node*> nodes = R.getNodes();
    std::set<transition*> transitions = R.getTransitions();
    std::set<transition*> eps_transitions = R.getEpsTransitions();
    // Create the new begin and end state
    Node* begin = new Node(beginName , true , false);
    Node* end = new Node(endName , false , true);
    nodes.insert(begin);
    nodes.insert(end);
    // Create new transitions
    // New begin node -> R begin node
    transition* newBeginTrans = new transition(begin , *R.getBegin().begin() , eps);
    transition* newEndTrans = new transition(*R.getFinal().begin() , end , eps);
    newBeginTrans->getEndNode()->setStarting(false);
    newEndTrans->getBeginNode()->setAccepting(false);
    // Loop transitions
    transition* newR_Loop = new transition(*R.getFinal().begin() , *R.getBegin().begin() , eps);
    transition* out_Loop = new transition(begin , end , eps);
    // Insert all the transitions
    transitions.insert(newBeginTrans);
    transitions.insert(newEndTrans);
    transitions.insert(newR_Loop);
    transitions.insert(out_Loop);
    eps_transitions.insert(newBeginTrans);
    eps_transitions.insert(newEndTrans);
    eps_transitions.insert(newR_Loop);
    eps_transitions.insert(out_Loop);
    // Create new ENFA
    ENFA* newENFA = new ENFA();
    newENFA->setEps(eps);
    newENFA->setAlphabet(alphabet);
    newENFA->setNodes(nodes);
    newENFA->setBegin({begin});
    newENFA->setFinal({end});
    newENFA->setTransitions(transitions);
    newENFA->setEpsTransitions(eps_transitions);
    
    return newENFA;
}

ENFA* RE::toENFA(std::string &reg , int &count){
    std::vector<ENFA*> temp;
    ENFA* newENFA;
    char c;
    char d;
    for(int i = 0; i < reg.size(); i++){
        // Get current character
        c= reg[i];
        // Get next character
        if(i != reg.size() - 1){
            d = reg[i+1];
        }
        else{
            d = ' ';
        }
        // Check if star operation
        if(c != eps && c != '*' && d == '*'){
            newENFA = createSingleChar(std::to_string(count) , std::to_string(count + 1) , c);
            count += 2;
            newENFA = createStar(std::to_string(count), std::to_string(count+1), *newENFA);
        }
        else if(c != eps && c != '*'){
            newENFA = createSingleChar(std::to_string(count) , std::to_string(count + 1) , c);
        }
        else{
            newENFA = createEpsilon( std::to_string(count) , std::to_string(count + 1) );
        }
        temp.push_back( newENFA );
        count += 2;
    }
    // Concatenate the small vector
    newENFA = createConcatenation(temp);
    return newENFA;
}

std::vector<ENFA*> RE::splitRegex(std::string &reg , int &count , std::vector<int>&index){

    // Create containers
    std::vector<ENFA*> beginReg;
    std::vector<ENFA*> temp;
    ENFA* temp_n;
    ENFA* temp_m;
    std::vector<ENFA*> current;
    char c;
    char d;
    int rem;
    std::string rest;
    int oldCount = count;
    int start = 0;
    if(!index.empty()){
        start = index.back() + 1;
    }
    for(int i = start; i < reg.size(); i++){
        // Get current and next character
        c = reg[i];
        if(i != reg.size() - 1){
            d = reg[i+1];
        }
        else{
            d = ' ';
        }
        // If start of bracket
        if(c == '('){
            oldCount = count;
            count++;
            index.push_back(i);
            // Recursion
            temp = splitRegex(reg , count , index);
            // Insert the recursive ENFA into the vector
            current.push_back(createPlus(temp , count));
            count++;
            // Get the old index
            i = index.back();
            index.pop_back();
            if(i == reg.size()-1){
                beginReg.push_back(createConcatenation(current));
            }
            // Reset temp
            temp = {};
            continue;
        }
        // If end of bracket
        else if(c == ')'){
            beginReg.push_back(createConcatenation(current));
            index.back() = i;
            return beginReg;
        }
        // If next character is star
        else if(d == '*'){
            temp_n = createSingleChar(std::to_string(count+1) , std::to_string(count + 2) , c);
            temp_m = createStar(std::to_string(count), std::to_string(count+3), *temp_n);
            count += 4;
            current.push_back(temp_m);
            i++;
        }
        // If current character is star
        else if(c == '*'){
            temp_n = createStar(std::to_string(count), std::to_string(count+1), *current.back());
            current.back() = temp_n;
            count += 2;
        }
        // If plus operation
        else if(c == '+'){
            beginReg.push_back(createConcatenation(current));
            // Reset current
            current = {};
            continue;
        }
        // If current character is in the alphabet
        else if(c != eps && c != '*'){
            current.push_back(createSingleChar(std::to_string(count), std::to_string(count + 1), c));
            count += 2;
        }
        // Create epsilon ENFA
        else{
            current.push_back(createEpsilon(std::to_string(count), std::to_string(count + 1)));
            count += 2;
        }
        if(c == *reg.rbegin()){
            beginReg.push_back(createConcatenation(current));
        }
    }
    return beginReg;
}

ENFA RE::toENFA() {
    // "ab+bc+cdf+e"
    // "abc*d"
    // "ab+ab(c+d+f)g"
    // "ab(cd)*+e"
    // Vector of concatenation strings
    int count = 1;
    std::vector<int>index;
    std::vector<ENFA*> reg = splitRegex(RE::regex , count , index);
    std::vector<ENFA*> conc;
    ENFA* newENFA;
    // Link all the enfa's
    if(reg.size() > 1){
        return *createPlus(reg , count);
    }
    else{
        return *reg.front();
    }
}

RE::~RE() {

}
