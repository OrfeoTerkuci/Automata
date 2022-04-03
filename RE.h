#ifndef AUTOMATA_RE_H
#define AUTOMATA_RE_H

#include "ENFA.h"
#include <iostream>
#include <vector>
#include <set>

using namespace std;

class RE {
private:
    string regex;
    char eps;
public:
    // Constructor
    RE(const string &regex, char eps);
    RE(RE* refRE);
    RE();
    // Getters and setters
    const string &getRegex() const;
    void setRegex(const string &regex);
    char getEps() const;
    void setEps(char eps);
    // Basic automatons
    ENFA createEpsilon(string beginName , string endName);
    ENFA createEmpty(string beginName , string endName);
    ENFA createSingleChar(string beginName , string endName , char a);
    // Induction automatons
    ENFA createPlus(string beginName , string endName , ENFA &R , ENFA &S);
    ENFA createConcatenation(ENFA &R , ENFA &S);
    ENFA createStar(string beginName , string endName , ENFA &R);
    // Convert function
    void splitRegex(vector<string> beginReg , string reg);
    ENFA toENFA();
    // Destructor
    virtual ~RE();
};


#endif //AUTOMATA_RE_H
