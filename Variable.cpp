#include "Variable.h"

Variable::Variable(const std::string &name, const std::vector<std::vector<Variable*> > &production , bool starting ,
                   bool terminal) :
                    name(name), production(production) , starting(starting) , terminal(terminal) {}

Variable::Variable() : production({}) , starting(false) , terminal(false) {}

const std::string &Variable::getName() const {
    return name;
}

void Variable::setName(const std::string &newName) {
    Variable::name = newName;
}

const std::vector<std::vector<Variable*> > &Variable::getProductions() const {
    return production;
}

void Variable::setProductions(const std::vector<std::vector<Variable*> > &newProductions) {
    Variable::production = newProductions;
}

void Variable::addProduction(std::vector<Variable*> newProduction) {
    if(newProduction.empty()){
        production.insert(production.begin() , {new Variable()});
    }
    else{
        production.push_back(newProduction);
    }
}

void Variable::addProduction(std::vector<std::string> newProduction) {
    if(newProduction.empty()){
        production.insert(production.begin() , {new Variable()});
    }
    else{
        std::vector<Variable*> newProd;
        for(const auto& v : newProduction){
            newProd.push_back(new Variable(v , {}));
        }
        production.push_back(newProd);
    }
}

bool Variable::isStarting() const {
    return starting;
}

void Variable::setStarting(bool newStarting) {
    Variable::starting = newStarting;
}

std::string Variable::getProduction(std::vector<std::string> &prod) {
    std::string tempName;
    for(const auto& s : prod){
        tempName += s;
        tempName += " ";
    }
    return tempName;
}

bool Variable::isTerminal() const {
    return terminal;
}

void Variable::setTerminal(bool newTerminal) {
    Variable::terminal = newTerminal;
}
