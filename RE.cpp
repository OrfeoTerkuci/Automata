#include "RE.h"
#include "Node.h"
#include "transition.h"
#include "transitionNFA.h"
#include <algorithm>
#include <string>
#include <vector>

RE::RE(const string &regex, char eps) : regex(regex), eps(eps) {
    // Get the alphabet
    for(char c : regex){
        if(c == '+' || c == '*' || c == '(' || c == ')'){
            continue;
        }
        else{
            alphabet.insert(c);
        }
    }
}

RE::RE(RE* refRE) : regex(refRE->getRegex()) , eps(refRE->getEps()) , alphabet(refRE->getAlphabet()) {}

RE::RE() : regex(" ") , eps(' ') , alphabet({}) {}

const string &RE::getRegex() const {
    return regex;
}

void RE::setRegex(const string &regex) {
    RE::regex = regex;
}

char RE::getEps() const {
    return eps;
}

void RE::setEps(char eps) {
    RE::eps = eps;
}

set<char> RE::getAlphabet() const{
    return alphabet;
}

void RE::setAlphabet(set<char> &newAlphabet){
    alphabet = newAlphabet;
}

ENFA* RE::createEpsilon(string beginName , string endName) {
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

ENFA* RE::createEmpty(string beginName , string endName) {
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

ENFA* RE::createSingleChar(string beginName , string endName , char a) {
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

ENFA* RE::createPlus(string beginName , string endName , vector<ENFA*> &ref) {
    // Create begin and end states
    Node* begin = new Node(beginName , true , false);
    Node* end = new Node(endName , false , true);
    // Create containers
    set<char> alphabet;
    set<Node*> nodes = {begin , end};
    set<transition*> transitions;
    set<transition*> eps_transitions;
    // Create new ENFA
    ENFA* newENFA = new ENFA();
    // Link the new begin and end node to all ENFAs
    for(auto &e : ref){

        auto* newTransR_begin = new transition(begin , *e->getBegin().begin(), eps);
        newTransR_begin->getEndNode()->setStarting(false);
        transitions.insert(newTransR_begin);
        
        auto* newTransR_end = new transition(*e->getFinal().begin() , end, eps);
        newTransR_end->getBeginNode()->setAccepting(false);
        transitions.insert(newTransR_end);

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
    }
    
    // Set all parameters
    newENFA->setEps(eps);
    newENFA->setAlphabet(alphabet);
    newENFA->setNodes(nodes);
    newENFA->setBegin({begin});
    newENFA->setFinal({end});   
    newENFA->setTransitions(transitions);
    // Get epsilon transitions
    for(transition* t : transitions){
        if(t->getInput() == newENFA->getEps()){
            eps_transitions.insert(t);
        }
    }
    newENFA->setEpsTransitions(eps_transitions);
    return newENFA;
}

ENFA* RE::createConcatenation(ENFA &R , ENFA &S) {
    // Create containers
    set<char> alphabet;
    set<Node*> nodes;
    set<transition*> transitions;
    set<transition*> eps_transitions;
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

ENFA* RE::createConcatenation(vector<ENFA*> ref){
// Create containers
    set<char> alphabet;
    set<Node*> nodes;
    set<transition*> transitions;
    set<transition*> eps_transitions;
    // Create new ENFA
    ENFA* newENFA = new ENFA();
    for(int i = 0; i < ref.size(); i++){
        // Get two current ENFA
        ENFA* first = ref[i];
        ENFA* second;
        // Make new transition
        transition* newTrans;
        // Get alphabet
        for(char c : first->getAlphabet()){
            alphabet.insert(c);
        }
        // If not the last enfa
        if(i != ref.size() - 1){
            second = ref[i+1];
            // Create new epsilon transition
            newTrans = new transition(*first->getFinal().begin() , *second->getBegin().begin() , eps);
            transitions.insert(newTrans);
            newTrans->getBeginNode()->setAccepting(false);
            newTrans->getEndNode()->setStarting(false);
            eps_transitions.insert(newTrans);
        }
        // Copy nodes
        for(auto n : first->getNodes()){
            nodes.insert(n);
        }
        // Copy transitions
        for(auto t : first->getTransitions()){
            transitions.insert(t);
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

ENFA* RE::createStar(string beginName , string endName , ENFA &R) {
    // Create new containers
    set<Node*> nodes = R.getNodes();
    set<transition*> transitions = R.getTransitions();
    set<char> alphabet;
    // Create the new begin and end state
    Node* begin = new Node(beginName , true , false);
    Node* end = new Node(endName , false , true);
    nodes.insert(begin);
    nodes.insert(end);
    // Create new transitions
    transition* newBeginTrans = new transition(begin , *R.getBegin().begin() , eps);
    transition* newEndTrans = new transition(*R.getFinal().begin() , end , eps);
    newBeginTrans->getEndNode()->setStarting(false);
    newEndTrans->getBeginNode()->setAccepting(false);
    transition* newR_Loop = new transition(*R.getFinal().begin() , *R.getBegin().begin() , eps);
    transition* out_Loop = new transition(begin , end , eps);
    transitions.insert(newBeginTrans);
    transitions.insert(newEndTrans);
    transitions.insert(newR_Loop);
    transitions.insert(out_Loop);
    // Create new ENFA
    ENFA* newENFA = new ENFA();
    newENFA->setEps(eps);
    newENFA->setAlphabet(R.getAlphabet());
    newENFA->setNodes(nodes);
    newENFA->setTransitions(transitions);
    newENFA->setBegin({begin});
    newENFA->setFinal({end});
    
    return newENFA;
}

ENFA* RE::toENFA(string &reg , int &count){
    vector<ENFA*> temp;
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
            newENFA = createSingleChar(to_string(count) , to_string(count + 1) , c);
            count += 2;
            newENFA = createStar(to_string(count), to_string(count+1), *newENFA);
        }
        else if(c != eps && c != '*'){
            newENFA = createSingleChar(to_string(count) , to_string(count + 1) , c);
        }
        else{
            newENFA = createEpsilon( to_string(count) , to_string(count + 1) );
        }
        temp.push_back( newENFA );
        count += 2;
    }
    // Concatenate the small vector
    newENFA = createConcatenation(temp);
    return newENFA;
}

vector<ENFA*> RE::splitRegex(string &reg , int &count){
    // "ab+bc+cdf+e"
    // "ab+ab(c+d)g"
    // "ab(cd)*"
    // "abc*d"
    vector<ENFA*> beginReg;
    vector<ENFA*> temp;
    ENFA* temp_n;
    vector<ENFA*> current;
    char c;
    char d;
    int rem;
    string rest;
    int oldCount = count;
    for(int i = 0; i < reg.size(); i++){
        // Get current and next character
        c = reg[i];
        if(i != reg.size() - 1){
            d = reg[i+1];
        }
        else{
            d = ' ';
        }
        if(c == '('){
            // Get remainder of string
            rem = reg.size() - i;
            rest = reg.substr(i+1 , rem);
            // Save count
            int oldCount = count;
            // Recursion
            temp = splitRegex(rest , count);
            // Insert the recursive ENFA into the vector
            current.push_back(createPlus(to_string(oldCount), to_string(count + 1), temp));
            rest = rest.substr(i+1 , count - oldCount);
            
            i += (count - oldCount);
            temp = {};
            continue;
        }
        else if(c == ')'){
            beginReg.push_back(createConcatenation(current));
            return beginReg;
        }
        else if(d == '*'){
            temp_n = createSingleChar(to_string(count) , to_string(count + 1) , c);
            count += 2;
            temp_n = createStar(to_string(count), to_string(count+1), *temp_n);
            count += 2;
            current.push_back(temp_n);
        }
        else if(c == '*'){
            current.back() = createStar(to_string(count), to_string(count+1), *current.back());
            count += 2;
        }
        else if(c == '+'){
            beginReg.push_back(createConcatenation(current));
            // count += 1;
            current = {};
            continue;
        }
        else if(c != eps && c != '*'){
            current.push_back(createSingleChar(to_string(count), to_string(count + 1), c));
            count += 2;
        }
        else{
            current.push_back(createEpsilon(to_string(count), to_string(count + 1)));
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
    vector<ENFA*> reg = splitRegex(RE::regex , count);
    vector<ENFA*> conc;
    ENFA* newENFA;
    // Link all the enfa's
    newENFA = createPlus("0" , to_string(count + 1) , reg);
    return *newENFA;
}

RE::~RE() {

}
