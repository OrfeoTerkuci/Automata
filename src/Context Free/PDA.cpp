#include "PDA.h"
#include "Variable.h"
#include "json.hpp"
#include "State.h"
#include "Transition.h"
#include <fstream>
#include <functional>

using json = nlohmann::json;

PDA::PDA(const std::string &filename) {
//* Read from file
    std::ifstream input(filename);
    json j;
    input >> j;

    // Read the terminals
    for (json::iterator it = j.begin(); it != j.end(); ++it) {
        // Designate alphabet
        if(it.key() == "States"){
            std::vector<std::string> a = it.value();
            std::string name = j["StartState"];
            for(const std::string& s : it->get<std::vector<std::string>>()){
                auto newS = new State(s , (s == name));
                states.insert(newS);
                if(newS->isStarting()){
                    beginState = newS;
                }
            }
        }
        else if(it.key() == "Alphabet"){
            alphabet = {it.value().begin() , it.value().end()};
        }
        else if(it.key() == "StackAlphabet"){
            stackAlphabet = {it.value().begin() , it.value().end()};
        }
        else if(it.key() == "StartStack"){
            beginStack = it->get<std::string>();
        }
        else if(it.key() == "Transitions"){
            auto vec = it.value();
            for(auto t : vec){
                // Create new transition
                auto newT = new Transition();
                // Get input
                newT->setInput(t["input"]);
                // Get stack input
                newT->setStackTop(t["stacktop"]);
                // Get replacement
                newT->setStackReplacement(t["replacement"]);
                for(auto s : states){
                    // Get beginState state
                    if(s->getName() == t["from"]){
                        newT->setBegin(s);
                    }
                    // Get endState state
                    if(s->getName() == t["to"]){
                        newT->setEnd(s);
                    }
                }
                transitions.push_back(newT);
            }
        }
    }

}

PDA::PDA() : states({}) , alphabet({}) , stackAlphabet({}) , transitions({}) , beginState(nullptr) {}

CFG PDA::toCFG() {
    // Create new CFG
    CFG res = CFG();
    // Copy alphabet over as terminals
    std::vector<Variable*> newVars;
    for(const auto& s : alphabet){
        newVars.push_back(new Variable(s , {} , false , true));
    }
    res.setTerminals(newVars);
    newVars.clear();
    // Create and set starting variable
    newVars = { new Variable("S" , {} , true) };
    res.setVariables( newVars );
    res.setStartingVar(res.getVariables()[0]);
    // Create the productions for starting symbol
    // S -> [q0Z0p] : p in Q
    for(const auto& p : states){
        std::string name = "[" + beginState->getName() + "," + beginStack + "," + p->getName() + "]";
        auto newVar = new Variable(name);
        newVars.push_back(newVar);
        res.getStartingVar()->addProduction({newVar});
    }
    // Create the new variables from the combinations
    for(const auto& s1 : states){
        for(const auto& s2 : states){
            for(const auto& sym : stackAlphabet){
                std::string name = "[" + s1->getName() + "," + sym + "," + s2->getName() + "]";
                for(auto st : newVars){
                    if(st->getName() == name){
                        break;
                    }
                    if(st == *newVars.rbegin()){
                        newVars.push_back(new Variable(name));
                    }
                }
            }
        }
    }
    // Create map for new transitions
    std::map< Transition* , std::vector< std::vector<std::pair<State* , State*> > > > newTransitions;
    // Initialize vectors
    for(auto t : transitions){
        if(t->getStackReplacement().empty()){
            newTransitions[t] = {{}};
        }
        else{
            newTransitions[t] = std::vector<std::vector<std::pair<State* , State*>>>(
                    (int)pow( 2 , (int)states.size() ) ,
                    std::vector<std::pair<State* , State*>>(
                            t->getStackReplacement().size() ,
                            std::pair<State* , State*>(nullptr , nullptr)
                            )
                    );
        }
    }
    // Create the rest of the new states
    for(auto t : transitions){
        std::vector<std::string> stackRep = t->getStackReplacement();
        if(stackRep.empty()){
            continue;
        }
        auto current_first = t->getEnd();
        auto current_last = states.begin();
        auto last_state = states.begin();
        for (int i = 0; i < newTransitions[t][0].size(); ++i) {
            for (int j = 0; j < newTransitions[t].size(); ++j) {
                // First element : reset first state
                if(i == 0){
                    current_first = t->getEnd();
                }
                else{
                    current_first = *current_last;
                }
                for (int k = 0; k < states.size(); ++k) {
                    for (int l = 0; l < states.size(); ++l) {
                        newTransitions[t][j][i].first = current_first;
                        newTransitions[t][j][i].second = *current_last;
                        if(i != newTransitions[t][0].size() - 1){
                            newTransitions[t][j][i+1].first = *current_last;
                            newTransitions[t][j][i+1].second = *last_state;
                            *last_state == *states.rbegin() ? last_state = states.begin() : last_state++;
                        }
                        // Advance end state
                        j++;
                    }
//                    last_state = states.begin();
                    *current_last == *states.rbegin() ? current_last = states.begin() : current_last++;
                    current_first = t->getEnd();
                }
                i += 2;
                j--;
            }
        }
    }
    // Assign the transitions
    for(const auto& t : newTransitions){
        Variable* currentVar;
        std::vector<Variable*> newProd;
        std::string name;
        for(const auto& s : t.second){
            // Get terminal
            for(auto term : res.getTerminals()){
                if(term->getName() == t.first->getInput()){
                    currentVar = term;
                    newProd.push_back(currentVar);
                    break;
                }
            }
            if(s.empty()){
                // Create new terminal transition
                name = "[" + t.first->getBegin()->getName() + "," + t.first->getStackTop() + "," + t.first->getEnd()->getName() + "]";
            }
            else{
                for (int i = 0; i < s.size(); ++i) {
                    name = "[" + s[i].first->getName() + "," + t.first->getStackReplacement()[i] + "," + s[i].second->getName() + "]";
                    for(auto st : newVars){
                        if(st->getName() == name){
                            newProd.push_back(st);
                            break;
                        }
                    }
                }
                name = "[" + t.first->getBegin()->getName() + "," + t.first->getStackTop() + "," + s.back().second->getName() + "]";
            }
            // Add production
            for(auto st : newVars){
                if(st->getName() == name){
                    st->addProduction(newProd);
                    newProd.clear();
                    break;
                }
            }
        }
    }
    std::sort( newVars.begin() , newVars.end() , [](Variable* x1 , Variable* x2){ return *x1 < *x2; } );
    res.setVariables(newVars);
    return res;
}
