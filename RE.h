//
// Created by student on 30.03.22.
//

#ifndef AUTOMATA_RE_H
#define AUTOMATA_RE_H

#include "ENFA.h"
#include <iostream>
#include <vector>
#include <set>

using namespace std;

class Node;
class transition;
class transitionNFA;

class RE {
private:
    string regex;
    char eps;
public:
    // Constructor
    RE(const string &regex, char eps);
    // Getters and setters
    const string &getRegex() const;
    void setRegex(const string &regex);
    char getEps() const;
    void setEps(char eps);
    // Basic automatons
    ENFA createEpsilon();
    ENFA createEmpty();
    ENFA createSingleChar(char a);
    // Induction automatons
    ENFA createPlus();
    ENFA createConcatenation();
    ENFA createStar();
    // Convert function
    ENFA toENFA();
    // Destructor
    virtual ~RE();
};


#endif //AUTOMATA_RE_H
