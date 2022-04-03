#include "RE.h"
#include "Node.h"
#include "transition.h"
#include "transitionNFA.h"

RE::RE(const string &regex, char eps) : regex(regex), eps(eps) {}

RE::RE(RE* refRE) : regex(refRE->getRegex()) , eps(refRE->getEps()) {}

RE::RE() : regex(" ") , eps(' ') {}

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

ENFA RE::createEpsilon(string beginName , string endName) {
    // Create states
    Node* begin = new Node(beginName , true , false);
    Node* end = new Node(endName , false , true);
    // Create epsilon transition
    auto * newTrans = new transition(begin , end , eps);
    // Create new ENFA
    ENFA newENFA = ENFA();
    // Set all parameters
    newENFA.setEps(eps);
    newENFA.setAlphabet({});
    newENFA.setNodes({begin , end});
    newENFA.setBegin({begin});
    newENFA.setFinal({end});
    newENFA.setTransitions({newTrans});
    return newENFA;
}

ENFA RE::createEmpty(string beginName , string endName) {
    // Create states
    Node* begin = new Node(beginName , true , false);
    Node* end = new Node(endName , false , true);
    // Create new ENFA
    ENFA newENFA = ENFA();
    // Set all parameters
    newENFA.setEps(eps);
    newENFA.setAlphabet({});
    newENFA.setNodes({begin , end});
    newENFA.setBegin({begin});
    newENFA.setFinal({end});
    newENFA.setTransitions({});
    return newENFA;
}

ENFA RE::createSingleChar(string beginName , string endName , char a) {
    // Create states
    Node* begin = new Node(beginName , true , false);
    Node* end = new Node(endName , false , true);
    // Create epsilon transition
    auto * newTrans = new transition(begin , end , a);
    // Create new ENFA
    ENFA newENFA = ENFA();
    // Set all parameters
    newENFA.setEps(eps);
    newENFA.setAlphabet({a});
    newENFA.setNodes({begin , end});
    newENFA.setBegin({begin});
    newENFA.setFinal({end});
    newENFA.setTransitions({newTrans});
    return newENFA;
}

ENFA RE::createPlus(string beginName , string endName , ENFA &R , ENFA &S) {
    // Create begin and end states
    Node* begin = new Node(beginName , true , false);
    Node* end = new Node(endName , false , true);
    // Create containers
    set<char> alphabet;
    set<Node*> nodes = {begin , end};
    set<transition*> transitions;
    // Link the begin node to both NFAs
    auto* newTransR_begin = new transition(begin , *R.getBegin().begin(), eps);
    auto* newTransS_begin = new transition(begin , *S.getBegin().begin(), eps);
    newTransR_begin->getEndNode()->setStarting(false);
    newTransS_begin->getEndNode()->setStarting(false);
    transitions.insert(newTransR_begin);
    transitions.insert(newTransS_begin);
    // Link the end node to both NFAs
    auto* newTransR_end = new transition(begin , *R.getFinal().begin(), eps);
    auto* newTransS_end = new transition(begin , *S.getFinal().begin(), eps);
    newTransR_end->getBeginNode()->setAccepting(false);
    newTransS_end->getBeginNode()->setAccepting(false);
    transitions.insert(newTransR_end);
    transitions.insert(newTransS_end);
    // Create new ENFA
    ENFA newENFA = ENFA();
    // Set all parameters
    newENFA.setEps(eps);
    // Copy the alphabet
    for(char c : R.getAlphabet()){
        alphabet.insert(c);
    }
    for(char c : S.getAlphabet()){
        alphabet.insert(c);
    }
    newENFA.setAlphabet(alphabet);
    // Copy all the nodes
    for( Node* n : R.getNodes()){
        nodes.insert(n);
    }
    for( Node* n : S.getNodes()){
        nodes.insert(n);
    }
    newENFA.setNodes(nodes);
    newENFA.setBegin({begin});
    newENFA.setFinal({end});
    // Copy transitions
    for(transition* t : R.getTransitions()){
        transitions.insert(t);
    }
    for(transition* t : S.getTransitions()){
        transitions.insert(t);
    }   
    newENFA.setTransitions(transitions);
    return newENFA;
}

ENFA RE::createConcatenation(ENFA &R , ENFA &S) {
    // Create containers
    set<char> alphabet;
    set<Node*> nodes;
    set<transition*> transitions;
    // Create new ENFA
    ENFA newENFA = ENFA();
    // Create new transition
    transition* newTrans = new transition(*R.getFinal().begin() , *S.getBegin().begin() , eps);
    transitions.insert(newTrans);
    // Set all parameters
    newENFA.setEps(eps);
    newENFA.setBegin(R.getBegin());
    newENFA.setFinal(S.getFinal());
    newTrans->getBeginNode()->setAccepting(false);
    newTrans->getEndNode()->setStarting(false);
    // Copy alphabet
    for(char c : R.getAlphabet()){
        alphabet.insert(c);
    }
    for(char c : S.getAlphabet()){
        alphabet.insert(c);
    }
    newENFA.setAlphabet(alphabet);
    // Copy nodes
    for(Node* n : R.getNodes()){
        nodes.insert(n);
    }
    for(Node* n : S.getNodes()){
        nodes.insert(n);
    }
    newENFA.setNodes(nodes);
    // Copy transitions
    for(transition* t : R.getTransitions()){
        transitions.insert(t);
    }
    for(transition* t : S.getTransitions()){
        transitions.insert(t);
    }
    newENFA.setTransitions(transitions);
    return newENFA;
}

ENFA RE::createStar(string beginName , string endName , ENFA &R) {
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
    ENFA newENFA = ENFA();
    newENFA.setEps(eps);
    newENFA.setAlphabet(R.getAlphabet());
    newENFA.setNodes(nodes);
    newENFA.setTransitions(transitions);
    newENFA.setBegin({begin});
    newENFA.setFinal({end});
    
    return newENFA;
}

void RE::splitRegex(vector<string> beginReg , string reg){
        
}

ENFA RE::toENFA() {
    // "(m+y)*+(e+y+m+i)s"
    vector<string> reg;
    splitRegex(reg , RE::regex);
    return ENFA();
}

RE::~RE() {

}
