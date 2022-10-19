#include "CFG.h"
#include "Variable.h"
#include <fstream>
#include <iomanip>
#include <functional>
#include "json.hpp"

using json = nlohmann::json;

bool compareVariables(Variable* a, Variable* b) { return (*a < *b); }

bool compareVector(std::vector<Variable*> a , std::vector<Variable*> b) {
    if(a.size() == b.size()){
        for(int i = 0; i < a.size(); i++){
            if (a[i] != b[i]) {
                return compareVariables(a[i] , b[i]);
            }
        }
    }
    return false;
}

CFG::CFG(std::string filename) {
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
                terminals.push_back(new Variable(s , {} , false , true));
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

}

CFG::CFG() {
    // Create the terminals
    terminals = { new Variable("0",{},false,true) ,
                  new Variable("1",{},false,true) ,
                  new Variable("a",{},false,true) ,
                  new Variable("b",{},false,true) };
    Variable* newVar;
    newVar = new Variable("BINDIGIT" , { {new Variable("0")} , {new Variable("1")} });
    variables.push_back(newVar);
    newVar = new Variable("S" , { "" , "a S b BINDIGIT"} , true );
    variables.push_back(newVar);
    startingVar = newVar;
}

void CFG::toCNF() {
    // Print original CFG
    std::cout << "Original CFG:\n\n";
    print();
    std::cout << "\n-------------------------------------\n" ;
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
    std::cout << "  Created " << newSize << " productions , original had " << oldSize << "\n\n";
    print();
    std::cout << std::endl;
}

std::vector<Variable*> CFG::calculateNullables() {
    // Create container
    std::set<Variable*> nullVar = {};
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
            p.clear();
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

}

void CFG::eliminateUseless() {

}

void CFG::fixTerminals() {

}

void CFG::fixVariables() {

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
            current += "    " + v->getName() + " -> `" + Variable::getProduction(p) + "` \n";
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
