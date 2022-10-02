#include "CFG.h"
#include "Variable.h"
#include <fstream>
#include <iomanip>
#include "json.hpp"

using json = nlohmann::json;

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
            for(std::string s : a){
                terminals.insert(s[0]);
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
    for(auto v : prods){
        // Search for the variable
        if(v["body"].empty()){
            productions[v["head"]].insert(productions[v["head"]].begin() , {""});
            continue;
        }
        productions[v["head"]].push_back(v["body"]);
    }

    // Set starting variable
    auto s = j["Start"];
    for(auto v : variables){
        if(v->getName() == s){
            v->setStarting(true);
            startingVar = v;
        }
    }

    std::sort(variables.begin() , variables.end());

}

CFG::CFG() : terminals({ '0' , '1' , 'a' , 'b' }) {
    // Create the terminals
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
    current = "V = {";
    for(const auto& t : terminals){
        current += t;
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
