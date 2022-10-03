#include "CFG.h"
#include "Variable.h"
#include <fstream>
#include <iomanip>
#include <algorithm>
#include <functional>
#include "json.hpp"

using json = nlohmann::json;

bool compareVariables(Variable* a, Variable* b) { return (*a < *b); }

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
                v->addProduction(p["body"]);
            }
        }
        // Add production to map
        if(p["body"].empty()){
            productions[p["head"]].insert(productions[p["head"]].begin() , {""});
            continue;
        }
        productions[p["head"]].push_back(p["body"]);
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
    productions[newVar->getName()] = { {"0"} , {"1"}};
    newVar = new Variable("S" , { "" , "a S b BINDIGIT"} , true );
    variables.push_back(newVar);
    productions[newVar->getName()] = { {""} , {"a S b BINDIGIT"} };
    startingVar = newVar;
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
    for(const auto& v : productions){
        for(auto p : v.second){
            current += '\t';
            current += v.first;
            current += " -> ";
            current += '\'';
            if(!p.empty()){
                for(const auto& r : p){
                    current += r;
                    if(r != *p.rbegin()){
                        current += " ";
                    }
                }
            }
            current += '\'';
            current += '\n';
        }
    }
    current += '}';
    current += '\n';
    std::cout << current;
    std::cout << "S = " << startingVar->getName() << std::endl;


}

CFG::~CFG() = default;
