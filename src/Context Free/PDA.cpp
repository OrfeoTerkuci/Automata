#include "PDA.h"
#include "json.hpp"
#include "State.h"
#include "Transition.h"
#include <fstream>
#include <functional>
#include <iomanip>

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
    return CFG();
}
