#include "CFG.h"
#include "Variable.h"
#include <fstream>
#include <iomanip>
#include <functional>
#include "json.hpp"

using json = nlohmann::json;

bool compareVariables(Variable* a, Variable* b) { return (*a < *b); }

bool compareVector(std::vector<Variable*> a , std::vector<Variable*> b) {
    std::vector<Variable*> v(a.begin(), a.begin() + (int)b.size());
    std::vector<Variable*> v1(b.begin(), b.begin() + (int)a.size());
    if(b == v && b.size() > 1){
        return false;
    }
    if(a == v1 && a.size() > 1){
        return false;
    }
    for(int i = 0; i < std::min(a.size() , b.size()); i++){
        if (a[i] != b[i]) {
            return compareVariables(a[i] , b[i]);
        }
        else if(a[i] == b[i] && i == std::min(a.size() , b.size()) - 1){
            return std::min(a.size() , b.size()) == b.size();
        }
    }
    return false;
}

CFG::CFG(const std::string& filename) {
    //* Read from file
    std::ifstream input(filename);
    json j;
    input >> j;

    // Read the terminals
    for (json::iterator it = j.begin(); it != j.end(); ++it) {
        // Designate alphabet
        if(it.key() == "Terminals"){
            std::vector<std::string> a = it.value();
            for(const std::string& s : a){
                terminals.push_back(new Variable(s , {} , false , true , true));
            }
        }
        else if(it.key() == "Variables"){
            std::vector<std::string> a = it.value();
            for(const std::string& s : a){
                variables.insert(variables.begin() , new Variable(s , {}));
            }
        }
    }

    // Read the productions
    auto prods = j["Productions"];
    for(auto p : prods){
        // Add production to variable
        for(auto v : variables){
            if(p["head"] == v->getName()){
                v->addProduction(p["body"] , variables , terminals);
            }
        }
    }

    // Set starting variable
    auto s = j["Start"];
    for(auto v : variables){
        if(v->getName() == s){
            v->setStarting(true);
            startingVar = v;
        }
    }

    std::sort(variables.begin() , variables.end() , compareVariables);
    std::sort(terminals.begin() , terminals.end() , compareVariables);
    sortProductions();

}

CFG::CFG() : variables({}) , terminals({}) , startingVar(nullptr) {}

void CFG::sortProductions() {
    for(auto v : variables){
        auto vec = v->getProductions();
        std::sort(vec.begin() , vec.end() , compareVector);
        v->setProductions(vec);
    }
}

void CFG::toCNF() {
    // Print original CFG
    std::cout << "Original CFG:\n\n";
    print();
    std::cout << "\n-------------------------------------\n" << std::endl;
    // Eliminate epsilon productions
    eliminateEpsilon();
    eliminateUnitPairs();
    eliminateUseless();
    fixTerminals();
    fixVariables();
}

void CFG::eliminateEpsilon() {
    // Print introduction
    std::cout << " >> Eliminating epsilon productions" << std::endl;
    std::vector<Variable*> nullVar = calculateNullables();
    std::cout << "  Nullables are {";
    for(auto v : nullVar){
        std::cout << v->getName();
        if(v != *nullVar.rbegin()){
            std::cout << ", ";
        }
    }
    std::cout << "}" << std::endl;
    int oldSize = 0;
    for(const auto &v : variables){
        oldSize += (int)v->getProductions().size();
    }
    for(auto &v : nullVar){
        fixNullable(v);
    }
    int newSize = 0;
    for(const auto &v : variables){
        newSize += (int)v->getProductions().size();
    }
    std::cout << "  Created " << newSize << " productions, original had " << oldSize << "\n" << std::endl;
    print();
    std::cout << std::endl;
}

std::vector<Variable*> CFG::calculateNullables() {
    std::set<Variable*> nullVar = {};
    for(auto& v : variables){
        if(v->isNullVar()){
            nullVar.insert(v);
        }
    }
    // Create container
    bool eval = true;
    while(eval){
        int oldSize = (int)nullVar.size();
        for(auto& v : variables){
            if(v->isNullVar()){
                nullVar.insert(v);
            }
        }
        eval = oldSize != nullVar.size();
    }
    return {nullVar.begin() , nullVar.end()};
}

void CFG::fixNullable(Variable* &var) {
    std::vector<std::vector<Variable*>> prods;
    std::vector<Variable*> newProd;
    std::vector<Variable*> editProd;
    for(auto &p : var->getProductions()){
        newProd = {};
        editProd = p;
        // Add original production
        if(!p.empty() && p[0]->getName().empty()){
            delete p[0];
            continue;
        }
        else if(!p.empty() && !p[0]->getName().empty()){
            prods.push_back(p);
        }
        for(int i = 0; i < p.size(); i++){
            // Add production without nullable variable
            if(p[i]->isNullable()){
                editProd.erase(editProd.begin() + i);
                if(!editProd.empty()){
                    prods.push_back(editProd);
                }
                editProd = p;
            }
            else if(!p[i]->isNullable() && !p[i]->isTerminal()){
                newProd.push_back(p[i]);
            }
        }
        if(!newProd.empty()){
            prods.push_back(newProd);
        }

    }
    for(auto it = prods.begin(); it != prods.end(); it++){
        if((*it)[0]->getName().empty()){
            it = prods.erase(it);
        }
    }
    std::sort(prods.begin() , prods.end() , compareVector);
    var->setProductions(prods);

}

void CFG::eliminateUnitPairs() {
    int totalPairs = 0;
    int oldSize = 0;
    for(const auto &v : variables){
        oldSize += (int)v->getProductions().size();
        for(auto p : v->getProductions()){
            if(p.size() == 1 && !p[0]->isTerminal()){
                totalPairs++;
            }
        }
    }
    // Print introduction
    std::cout << " >> Eliminating unit pairs" << std::endl;
    std::set<std::pair<Variable* , Variable*>> unitPairs = calculateUnits();
    std::set<std::vector<Variable*>> newProd;
    std::vector<std::vector<Variable*>> v_prods;
    std::vector<std::vector<Variable*>> e_prods;
    std::cout << "  Found " << totalPairs << " unit productions" << std::endl;
    std::cout << "  Unit pairs: {";
    for(auto u : unitPairs){
        std::cout << "(" << u.first->getName() << ", " << u.second->getName() << ")";
        if(u != *unitPairs.rbegin()){
            std::cout << ", ";
        }
        else{
            std::cout << "}\n";
        }
        v_prods = u.second->getProductions();
        newProd.insert(v_prods.begin() , v_prods.end());
        std::vector<Variable*> sec = {u.second};
        v_prods = std::vector<std::vector<Variable*>>(newProd.begin() , newProd.end());
        if(u.first == u.second){
            newProd = {};
            continue;
        }
        auto vec = u.first->getProductions();
        // Get intersection
        std::set_intersection(vec.begin(),vec.end(),
                              v_prods.begin(),v_prods.end(),
                              back_inserter(e_prods));
        // Remove intersection from vector to be inserted
        vec = {};
        std::set_difference(v_prods.begin(), v_prods.end(), e_prods.begin(), e_prods.end(),
                            std::inserter(vec, vec.begin()));
        u.first->replaceProduction(sec , vec);
        newProd = {};
        e_prods = {};
        v_prods = {};
    }
    eliminateUnitProductions();
    sortProductions();
    int newSize = 0;
    for(const auto &v : variables){
        newSize += (int)v->getProductions().size();
    }
    std::cout << "  Created " << newSize << " new productions, original had " << oldSize << "\n" << std::endl;
    print();
    std::cout << std::endl;
}

void CFG::eliminateUnitProductions() {
    for(auto v : variables){
        for(auto p : v->getProductions()){
            if((p.size() == 1 && !p[0]->isTerminal()) || p.empty()){
                v->removeProduction(p);
            }
        }
    }
}

std::set<std::pair<Variable* , Variable*>> CFG::calculateUnits() {
    std::set<std::pair<Variable* , Variable*>> units;
    std::pair<bool , std::set<Variable*> > current_unit;
    for(auto v : variables){
        current_unit = v->isUnit();
        if(current_unit.first){
            for(auto v1 : current_unit.second){
                units.insert(std::pair<Variable* , Variable*>(v , v1));
            }
        }
        else{
            units.insert(std::pair<Variable* , Variable*>(v , *current_unit.second.begin()));
        }
    }
    return units;
}

void CFG::eliminateUseless() {
    // Store beginning sizes
    int oldVar = (int)variables.size();
    int oldTerm = (int)terminals.size();
    int oldProd = 0;
    for(auto v : variables){
        oldProd += (int)v->getProductions().size();
    }
    // Introduction print
    std::cout << " >> Eliminating useless symbols" << std::endl;
    // Calculate generating variables
    std::vector<Variable*> genVar = calculateGenerating();
    std::sort(genVar.begin() , genVar.end() , compareVariables);
    std::cout << "  Generating symbols: {";
    for(auto v : genVar){
        std::cout << *v;
        if (v == *genVar.rbegin()) {
            std::cout << "}" << std::endl;
        } else {
            std::cout << ", ";
        }
    }
    // Eliminate non generating variables
    eliminateNonGenerating();
    // Calculate reachable variables
    std::vector<Variable*> reachVar = calculateReachable();
    std::sort(reachVar.begin() , reachVar.end() , compareVariables);
    std::cout << "  Reachable symbols: {";
    for(auto v : reachVar){
        std::cout << *v;
        if (v == *reachVar.rbegin()) {
            std::cout << "}" << std::endl;
        } else {
            std::cout << ", ";
        }
    }
    // Find useful variables
    std::vector<Variable*> usefulVar;
    std::set_intersection(genVar.begin() , genVar.end() ,
                        reachVar.begin() , reachVar.end() ,
                        std::inserter(usefulVar, usefulVar.end()));
    std::sort(reachVar.begin() , reachVar.end() , compareVariables);
    std::cout << "  Useful symbols: {";
    for(auto v : usefulVar){
        std::cout << *v;
        if (v == *usefulVar.rbegin()) {
            std::cout << "}" << std::endl;
        } else {
            std::cout << ", ";
        }
    }
    eliminateUnreachable(usefulVar);
    // Results output
    int newProd = 0;
    for(auto v : variables){
        newProd += (int)v->getProductions().size();
    }
    std::cout   << "  Removed " << oldVar - variables.size() << " variables, "
                << oldTerm - terminals.size() << " terminals "
                << "and " << oldProd - newProd << " productions \n"
                << std::endl;
    print();
    std::cout << std::endl;
}

std::vector<Variable *> CFG::calculateGenerating() {
    // Create container
    std::set<Variable*> genVar = {terminals.begin() , terminals.end()};
    bool eval = true;
    for(auto v : variables){
        v->isGeneratingVar();
        if(v->isGenerating()){
            genVar.insert(v);
        }
    }
    while(eval){
        int oldSize = (int)genVar.size();
        for(auto& v : variables){
            if(v->isGenerating()){
                genVar.insert(v);
            }
        }
        eval = oldSize != genVar.size();
    }
    return {genVar.begin() , genVar.end()};
}

void CFG::eliminateNonGenerating() {
    for(auto it = variables.begin(); it != variables.end(); it++){
        if(!(*it)->isGenerating()){
            it = variables.erase(it);
        }
        (*it)->eliminateNonGen();
    }
}

std::vector<Variable *> CFG::calculateReachable() {
    static std::set<Variable*> reachVar = {startingVar};
    int oldSize = (int)reachVar.size();
    for (auto v: reachVar) {
        for (const auto &p: v->getProductions()) {
            for (auto v1: p) {
                if (v1->isTerminal() || v1->isGenerating()) {
                    reachVar.insert(v1);
                }
            }
        }
    }
    if(oldSize == reachVar.size()){
        return {reachVar.begin() , reachVar.end()};
    }
    return calculateReachable();
}

void CFG::eliminateUnreachable(const std::vector<Variable *>& reachVars) {
    for(auto it = variables.begin(); it != variables.end(); it++){
        if((*it)->isTerminal()){
            continue;
        }
        if(!(std::find(reachVars.begin(), reachVars.end(), (*it)) != reachVars.end())){
            it = variables.erase(it);
        }
    }
    for(auto it = terminals.begin(); it != terminals.end(); it++){
        if(!(*it)->isTerminal()){
            continue;
        }
        if(!(std::find(reachVars.begin(), reachVars.end(), (*it)) != reachVars.end())){
            it = terminals.erase(it);
        }
    }
}

void CFG::fixTerminals() {
    std::vector<Variable*> newVars;
    std::map<Variable* , Variable*> terminalsExisting;
    int oldSize = 0;
    for(auto v : variables){
        oldSize += (int)v->getProductions().size();
    }
    // Find good bodies
    for(auto v : variables){
        for(auto p : v->getProductions()){
            if(p.size() == 1 && p[0]->isTerminal()){
                terminalsExisting[p[0]] = v;
            }
        }
    }
    // Fix bad bodies
    for(auto t : terminals){
        if(terminalsExisting[t] == nullptr){
            std::string newName = "_" + t->getName();
            auto newVar = new Variable(newName);
            newVar->addProduction({t});
            variables.push_back(newVar);
            terminalsExisting[t] = newVar;
            newVars.push_back(newVar);
        }
    }
    // Replace variables in bad bodies
    for(auto &v : variables){
        auto prod = v->getProductions();
        for(auto &p : prod){
            if(p.size() == 1){
                continue;
            }
            for(auto &v1 : p){
                // Replace with existing variable
                if(v1->isTerminal()){
                    v1 = terminalsExisting[v1];
                }
            }
        }
        v->setProductions(prod);
    }
    std::sort(newVars.begin() , newVars.end() , compareVariables);
    std::sort(variables.begin() , variables.end() , compareVariables);
    sortProductions();
    std::cout   << " >> Replacing terminals in bad bodies \n"
                << "  Added " << (int)newVars.size() << " new variables: {";
    for(auto v : newVars){
        std::cout << *v;
        if (v != *newVars.rbegin()) {
            std::cout << ", ";
        }
    }
    std::cout << "}\n";
    int newSize = 0;
    for(const auto &v : variables){
        newSize += (int)v->getProductions().size();
    }
    std::cout << "  Created " << newSize << " new productions, original had " << oldSize << "\n" << std::endl;
    print();
    std::cout << std::endl;
}

void CFG::fixVariables() {
    std::vector<Variable*> newVars;
    std::map<std::vector<Variable*> , Variable*> variablesExisting;
    int oldSize = (int)variables.size();
    // Find good bodies
    for(auto v : variables){
        for(const auto& p : v->getProductions()){
            if(p.size() == 2){
                variablesExisting[p] = v;
            }
        }
    }
    int varCount = 0;
    // Fix bad bodies
    for(auto v : variables){
        int el = 2;
        for(auto p : v->getProductions()){
            // If production too long
            if(p.size() > 2){
                int i = (int)p.size();
                // Check if new variable already exists
                if(variablesExisting[ {p[i-2] , p[i-1]} ] == nullptr || variablesExisting[ {p[i-2] , p[i-1]} ] == v ){
                    std::string newName = v->getName() + "_" + std::to_string(el);
                    auto newVar = new Variable(newName);
                    newVar->addProduction({p[i-2] , p[i-1]});
                    variables.push_back(newVar);
                    variablesExisting[{p[i-2] , p[i-1]}] = newVar;
                    newVars.push_back(newVar);
                    varCount++;
                    el++;
                }
            }
        }
    }
    int bodyCount = 0;
    // Replace variables in bad bodies
    for(auto &v : variables){
        auto prod = v->getProductions();
        for(auto &p : prod){
            if(p.size() == 1 && p[0]->isTerminal()){
                continue;
            }
            if(p.size() == 2){
                continue;
            }
            for(int i = (int)p.size() - 1; i > 0; i--){
                // Replace with existing variable
                if(variablesExisting[{p[i-1] , p[i]}] != nullptr){
                    p[i-1] = variablesExisting[{p[i-1] , p[i]}];
                    auto it = p.begin();
                    std::advance(it , i);
                    p.erase(it);
                    bodyCount++;
                }
            }
        }
        v->setProductions(prod);
    }
    std::sort(newVars.begin() , newVars.end() , compareVariables);
    std::sort(variables.begin() , variables.end() , compareVariables);
    sortProductions();
    int newSize = (int)variables.size();
    std::cout   << " >> Broke " << bodyCount << " bodies, added " << newSize - oldSize << " new variables\n"
                << ">>> Result CFG:\n"
                << std::endl;
    print();
}

void CFG::print() {
    std::string current;
    // Print variables
    current = "V = {";
    for(const auto& v : variables){
        current += v->getName();
        if(v != *variables.rbegin()){
            current += ", ";
        }
        else{
            current += "}";
        }
    }
    std::cout << current << std::endl;
    // Print terminals
    current = "T = {";
    for(const auto& t : terminals){
        current += t->getName();
        if(t != *terminals.rbegin()){
            current += ", ";
        }
        else{
            current += "}";
        }
    }
    std::cout << current << std::endl;
    // Print productions
    current = "P = {";
    current += '\n';
    for(auto v : variables){
        for(auto p : v->getProductions()){
            current += "  " + v->getName() + " -> `" + Variable::getProduction(p) + "` \n";
        }

    }
    current += '}';
    current += '\n';
    std::cout << current;
    std::cout << "S = " << startingVar->getName() << std::endl;


}

CFG::~CFG() {
    for(auto v : variables){
        delete v;
    }
    variables.clear();
    for(auto t : terminals){
        delete t;
    }
    terminals.clear();
}
