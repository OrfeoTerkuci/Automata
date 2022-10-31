#include "Variable.h"
#include <string>
#include <algorithm>
#include <utility>

bool Variable::prodExists(std::vector<Variable*> &newProduction){
    return std::any_of(production.begin() , production.end() , [&](const auto& p){return p == newProduction;});
}

Variable::Variable(std::string name, std::vector<std::vector<Variable*> > production , bool starting ,
                   bool terminal , bool generating) :
                    name(std::move(name)), production(std::move(production)) , starting(starting) ,
                    terminal(terminal) , nullable(false) , generating(generating) {}

Variable::Variable() : production({}) , starting(false) , terminal(false) , nullable(true) , generating(false) {}

const std::string &Variable::getName() const {
    return name;
}

void Variable::setName(const std::string &newName) {
    Variable::name = newName;
}

std::vector<std::vector<Variable*> > Variable::getProductions() const {
    return production;
}

void Variable::setProductions(const std::vector<std::vector<Variable*> > &newProductions) {
    Variable::production = newProductions;
}

void Variable::addProduction(std::vector<Variable*> newProduction) {
    if(prodExists(newProduction)){
        return;
    }
    if(newProduction.empty()){
        production.insert(production.begin() , {new Variable()});
    }
    else{
        production.push_back(newProduction);
    }
}

void Variable::removeProduction(std::vector<Variable *> &prod) {
    for(auto it = production.begin(); it != production.end(); it++){
        if(*it == prod){
            production.erase(it);
            return;
        }
    }
}

void Variable::replaceProduction(std::vector<Variable *> &prod, std::vector<std::vector<Variable *>> &newProd) {
    for(auto it = production.begin(); it != production.end(); it++){
        if(*it == prod){
            it = production.erase(it);
            production.insert(it , newProd.begin() , newProd.end());
            return;
        }
    }
}

void Variable::addProduction(const std::vector<std::string>& newProduction , std::vector<Variable*> &vars , std::vector<Variable*> &terms) {
    if(newProduction.empty()){
        production.insert(production.begin() , {new Variable()});
    }
    else{
        std::vector<Variable*> newProd;
        for(const auto& nv : newProduction){
            for(auto v : vars){
                if(nv == v->getName()){
                    newProd.push_back(v);
                }
            }
            for(auto v : terms){
                if(nv == v->getName()){
                    newProd.push_back(v);
                }
            }
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

bool Variable::operator==(const Variable &rhs) const {
    return name == rhs.name &&
           production == rhs.production &&
           starting == rhs.starting &&
           terminal == rhs.terminal;
}

bool Variable::operator!=(const Variable &rhs) const {
    return !(rhs == *this);
}

bool Variable::operator<(const Variable &rhs) const {
    if (name.empty()) return false;
    return rhs.name.empty() || !(rhs.name < name);
}

bool Variable::operator>(const Variable &rhs) const {
    return rhs < *this;
}

bool Variable::operator<=(const Variable &rhs) const {
    return !(rhs < *this);
}

bool Variable::operator>=(const Variable &rhs) const {
    return !(*this < rhs);
}

std::ostream &operator<<(std::ostream &os, const Variable &variable) {
    os << variable.name;
    return os;
}

std::string Variable::getProduction(std::vector<Variable *> &prod) {
    std::string output = "`";
    for(int i = 0; i < prod.size(); i++){
        output += prod[i]->getName();
        if(i != prod.size() - 1){
            output += " ";
        }
    }
    return output + "`";
}

bool Variable::isNullable() const {
    return nullable;
}

void Variable::setNullable(bool newNullStat) {
    Variable::nullable = newNullStat;
}

bool Variable::isNullVar() {
    if(name.empty() && !terminal){
        return true;
    }
    if(terminal){
        return false;
    }
    for(const auto& p : production){
        for(auto v : p){
            if(v == this){
                continue;
            }
            if(v->isNullable() && !v->isTerminal()){
                nullable = true;
                return true;
            }
//            if(v->isNullVar() && !v->isTerminal()){
//                nullable = true;
//                return true;
//            }
        }
    }
    return false;
}

std::pair<bool , std::set<Variable*> > Variable::isUnit() {
    if(terminal){
        return {false , {this} };
    }
    else {
        std::set<Variable*> r = {this};

        for (auto p: production) {
            if (p.size() == 1 && !p[0]->terminal) {
                std::set<Variable*> k = p[0]->isUnit().second;
                r.insert(k.begin() , k.end());
            }
        }
        return {r.size() > 1 , r};
    }
}

bool Variable::isGenerating() const {
    return generating;
}

void Variable::setGenerating(bool newGenerating) {
    Variable::generating = newGenerating;
}

bool Variable::isGeneratingVar() {
    if(terminal){
        generating = true;
        return true;
    }
    if(production.empty()){
        return false;
    }
    bool gen = true;
    while(gen){
        for(const auto& p : production){
            gen = true;
            for(auto v : p){
                if(v == this){
                    continue;
                }
                if(!v->isGenerating()){
                    gen = false;
                }
            }
            if(gen){
                generating = true;
                return true;
            }
        }
    }
    return gen;
}

void Variable::eliminateNonGen() {
    bool eval = true;
    while(eval){
        eval = false;
        for(auto it = production.begin(); it != production.end(); it++){
            for(auto v : *it){
                if(!v->isGenerating()){
                    it = production.erase(it);
                    eval = true;
                }
            }
        }
    }
}

bool Variable::hasProduction(const char &t) const {
    for(const auto& p : production){
        for(const auto& v : p){
            if(v->getName()[0] == t){
                return true;
            }
        }
    }
    return false;
}

bool Variable::hasProduction(const std::vector<Variable*>& p) const {
    return std::any_of(production.begin() , production.end() , [&](const std::vector<Variable*>& p1){ return p1 == p;});
}

Variable::~Variable() {
    for(const auto& p : production){
        for(auto v : p){
            if(v->getName().empty()){
                delete v;
            }
        }
    }
    production.clear();
}
