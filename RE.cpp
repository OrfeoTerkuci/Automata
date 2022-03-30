//
// Created by student on 30.03.22.
//

#include "RE.h"
#include "Node.h"
#include "transition.h"
#include "transitionNFA.h"

RE::RE(const string &regex, char eps) : regex(regex), eps(eps) {}

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

ENFA RE::createEpsilon() {
    // Create states
    Node* begin = new Node("" , true , false);
    Node* end = new Node("" , false , true);
    // Create epsilon transition
    auto * newTrans = new transition(begin , end , eps);
    // Create new ENFA
    ENFA newENFA = ENFA();
    // Set all parameters
    newENFA.setEps(eps);
    newENFA.setNodes({begin , end});
    newENFA.setBegin({begin});
    newENFA.setFinal({end});
    newENFA.setTransitions({newTrans});
    return ENFA();
}

ENFA RE::createEmpty() {
    return ENFA();
}

ENFA RE::createSingleChar(char a) {
    return ENFA();
}

ENFA RE::createPlus() {
    return ENFA();
}

ENFA RE::createConcatenation() {
    return ENFA();
}

ENFA RE::createStar() {
    return ENFA();
}

ENFA RE::toENFA() {
    return ENFA();
}

RE::~RE() {

}
