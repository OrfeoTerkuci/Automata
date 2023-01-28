#include "Variable.h"
#include <string>
#include <algorithm>
#include <utility>

bool Variable::prodExists(std::vector<Variable*> &newProduction){
    return std::any_of(production.begin() , production.end() ,
                       [&](const std::vector<Variable*>& p){return p == newProduction;});
}

Variable::Variable(const std::string& name, std::vector<std::vector<Variable*> > production , bool starting ,
                   bool terminal , bool generating) :
                    name(name), production(std::move(production)) , starting(starting) ,
                    terminal(terminal) , generating(generating) {
    if (terminal) firstVar = {this};
    nullable = name.empty();
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

std::string Variable::getProduction(std::vector<std::string> &prod) {
    std::string tempName;
    for(const auto& s : prod){
        tempName += s;
        tempName += " ";
    }
    return tempName;
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
    return !name.empty() && (rhs.name.empty() || !(rhs.name < name));
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

bool Variable::isNullVar() {
    if(name.empty()){
        return true;
    }
    if(terminal){
        return false;
    }
    for(const auto& p : production){
        for(auto& v : p){
            if(v == this){
                continue;
            }
            if(v->isNullable()){
                nullable = true;
                return true;
            }
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
    if(starting){
        starting = true;
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
                    break ;
                }
            }
        }
    }
}

void Variable::eliminateNonReach() {
    bool eval = true;
    while(eval){
        eval = false;
        for(auto it = production.begin(); it != production.end(); it++){
            for(auto v : *it){
                if(!v->isReachable()){
                    it = production.erase(it);
                    eval = true;
                    break ;
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


bool Variable::hasProduction(const std::string &t) const{
    for(const auto& p : production){
        for(const auto& v : p){
            if(v->getName() == t){
                return true;
            }
        }
    }
    return false;
}

bool Variable::hasProduction(const std::vector<Variable*>& p) const {
    return std::any_of(production.begin() , production.end() , [&](const std::vector<Variable*>& p1){ return p1 == p;});
}

std::set<Variable *> Variable::calculateFirst() {
    if(terminal){
        firstVar = {this};
        return {this};
    }
    // Initialize return set
    std::set<Variable*> firstSet;
    // Check if nullable
    isNullVar();
    if(nullable){
        // Get epsilon production
        for(auto p : production){
            if(p.size() == 1 && p[0]->getName().empty()){
                firstSet.insert(p[0]);
                firstVar.insert(p[0]);
            }
        }
    }
    for(const auto& p : production){
        std::set<Variable*> vec = first(p);
        firstSet.insert(vec.begin() , vec.end());
        firstVar.insert(vec.begin() , vec.end());
    }
    return firstSet;
}

std::set<Variable*> Variable::first(const std::vector<Variable*>& prod) {
    // prod = {Y1 , Y2 , Y3 , ... , Yn}
    std::set<Variable*> prod_set;
    std::set<Variable*> nextSet;
    std::set<Variable*> res;
    if(prod.size() == 1 && prod[0]->getName().empty()){
        firstVar = {prod[0]};
        return {prod[0]};
    }
    for(int i = 0; i < prod.size(); ++i){
        // Calculate FIRST(var)
        prod_set = prod[i]->calculateFirst();
        // If it doesn't contain epsilon
        if(!std::any_of(prod_set.begin() , prod_set.end() , [&](Variable* v){return v->getName().empty();})){
            firstVar = prod_set;
            prod[i]->firstVar = prod_set;
            return prod_set;
        }
        // If last element and contains epsilon
        if(i == prod.size() - 1){
            firstVar = prod_set;
            prod[i]->firstVar = prod_set;
            return prod_set;
        }
        // If it contains epsilon but not last element
        // Remove epsilon
        for (auto it = prod_set.begin(); it != prod_set.end(); it++) {
            if((*it)->getName().empty()){
                prod_set.erase(it);
                break;
            }
        }
        nextSet = prod[i+1]->calculateFirst();
        // FIRST(X) = { FIRST(Y1) – Є } U { FIRST(Y2) }
        std::set_union(prod_set.begin() , prod_set.end() , nextSet.begin() , nextSet.end() , std::inserter(res , res.begin()));
        firstVar = res;
        prod[i+1]->firstVar = res;
        return res;
    }
    firstVar = {};
    return {};
}
void Variable::follow(Variable* var , const std::vector<Variable*>& prod) {
    // Starting variable
    if(var->starting){
        if(!std::any_of(var->followVar.begin() , var->followVar.end() , [&](Variable* v){return v->getName() == "<EOS>";})){
            var->followVar.insert(new Variable("<EOS>"));
        }
    }
    Variable* currentVar;
    Variable* nextVar;
    for (int i = 0; i < prod.size(); ++i) {
        currentVar = prod[i];
        if(currentVar->terminal || currentVar->name.empty()){
            continue;
        }
        if(i != prod.size() - 1){
            nextVar = prod[i+1];
            auto vec = nextVar->firstVar;
            // Contains epsilon?
            if(std::any_of(vec.begin() , vec.end() , [&](Variable* v){return v->getName().empty();})){
                vec.erase(std::find_if(vec.begin() , vec.end() , [&](Variable* v){return v->getName().empty();}));
                currentVar->followVar.insert(vec.begin() , vec.end());
                int k = i + 2;
                while(k < prod.size()){
                    vec = prod[k]->firstVar;
                    if(!std::any_of(vec.begin() , vec.end() , [&](Variable* v){return v->getName().empty();})){
                        currentVar->followVar.insert(vec.begin() , vec.end());
                        break;
                    }
                    else{
                        vec.erase(std::find_if(vec.begin() , vec.end() , [&](Variable* v){return v->getName().empty();}));
                        currentVar->followVar.insert(vec.begin() , vec.end());
                    }
                    k++;
                }
                currentVar->followVar.insert(var->followVar.begin() , var->followVar.end());
            }
            else{
                currentVar->followVar.insert(vec.begin() , vec.end());
            }
        }
        else{
            currentVar->followVar.insert(var->followVar.begin() , var->followVar.end());
        }
    }
}
