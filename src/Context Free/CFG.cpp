#include "CFG.h"
#include "Variable.h"
#include <fstream>
#include <iomanip>
#include <functional>
#include "json.hpp"

using json = nlohmann::json;

bool compareVariables(Variable* a, Variable* b) { return (*a < *b); }

bool compareVector(std::vector<Variable*> a , std::vector<Variable*> b) {

    if(std::includes(a.begin() , a.end() , b.begin() , b.end()) && b.size() > 1){
        return false;
    }
    if(std::includes(b.begin() , b.end() , a.begin() , a.end()) && a.size() > 1){
        return false;
    }
    // Case `` vs something
    if(a.size() == 1 && a[0]->getName().empty() && !b.empty()){
        return b[0]->getName()[0] > '`';
    }
    // Case something vs ``
    else if(b.size() == 1 && b[0]->getName().empty() && !a.empty()){
        return a[0]->getName()[0] < '`';
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

const std::vector<Variable *> &CFG::getVariables() const {
    return variables;
}

void CFG::setVariables(const std::vector<Variable *> &newVars) {
    CFG::variables = newVars;
}

const std::vector<Variable *> &CFG::getTerminals() const {
    return terminals;
}

void CFG::setTerminals(const std::vector<Variable *> &newTerms) {
    CFG::terminals = newTerms;
}

Variable *CFG::getStartingVar() const {
    return startingVar;
}

void CFG::setStartingVar(Variable *newStartingVar) {
    CFG::startingVar = newStartingVar;
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
    std::map<std::vector<Variable*> , std::vector<Variable*>> variablesExisting;
    int oldSize = (int)variables.size();
    // Find good bodies
    for(auto v : variables){
        for(const auto& p : v->getProductions()){
            if(p.size() == 2){
                variablesExisting[p].push_back(v);
            }
        }
    }
    int varCount = 0;
    int bodyCount = 0;
    // Fix bad bodies
    for(auto v : variables) {
        int el = 2;
        auto prod = v->getProductions();
        for (auto &p: prod) {
            // If production too long
            while (p.size() > 2) {
                int i = (int) p.size();
                // Check if new variable already exists
                auto vec = variablesExisting[{p[i - 2], p[i - 1]}];
                auto v2 = std::find(vec.begin(), vec.end(), v);
                if (vec.empty() || v2 == vec.end() || *v2 == v) {
                    std::string newName = v->getName() + "_" + std::to_string(el);
                    auto newVar = new Variable(newName);
                    newVar->addProduction({p[i - 2], p[i - 1]});
                    variables.push_back(newVar);
                    variablesExisting[{p[i - 2], p[i - 1]}].push_back(newVar);
                    newVars.push_back(newVar);
                    varCount++;
                    el++;
                }
                for (int j = (int) p.size() - 1; i > 0; i--) {
                    // Replace with existing variable
                    vec = variablesExisting[{p[j - 1], p[j]}];
                    if (!vec.empty()) {
                        // Check which variable matches our name
                        for (auto v3: vec) {
                            std::string n = v->getName();
                            std::string n2 = v3->getName().substr(0, n.length());
                            if (n2 == n && v3 != v) {
                                p[j - 1] = v3;
                                auto it = p.begin();
                                std::advance(it, j);
                                it = p.erase(it);
                                bodyCount++;
                            }
                        }
                    }

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

void CFG::accepts(const std::string &input) {
    /*
| {A, C, S}  |
| {}         | {A, C, S}  |
| {}         | {B}        | {B}     |
| {A, S}     | {B}        | {C, S}  | {A, S}  |
| {B}        | {A, C}     | {A, C}  | {B}     | {A, C}  |
     */

    bool res = false;
    std::vector<int> max_elem = std::vector<int>(input.length() , 0);
    // Initialize table
    std::vector<std::vector<std::set<Variable*>>> table;
    for (int i = 0; i < input.size(); ++i) {
        std::vector<std::set<Variable*>> current(input.length() - i , std::set<Variable*>({}));
        table.push_back(current);
    }

    std::vector<std::set<Variable*>>& currentRow = table[0];
    std::vector<std::set<Variable*>> nextRow;
    // Initialize first row
    for(int i = 0; i < input.size(); i++){
        for(auto v : variables){
            if(v->hasProduction(input[i])){
                currentRow[i].insert(v);
            }
        }
    }
    // Start the algorithm
    for (int i = 1; i < table.size(); ++i) {
        for (int j = 0; j < table[i].size(); ++j) {
            // Triangulate
            std::set<Variable*> elementUnder;
            std::set<Variable*> elementDiag;
            std::set<Variable*> newElem = {};
            for (int k = 0; k < i; ++k) {
                // Get element under
                elementUnder = table[k][j];
                // Get element diagonally
                elementDiag = table[i - k - 1][j + k + 1];
                // Merge sets
                // Check for pairings
                for(auto v1 : elementUnder){
                    for(auto v2 : elementDiag){
                        for(auto v : variables){
                            if(v->hasProduction({v1 , v2})){
                                newElem.insert(v);
                            }
                        }
                    }
                }
                elementUnder = {};
                elementDiag = {};
                // Set new element
                table[i][j] = newElem;
            }
        }
    }
    // Print table
    // Get max width of table cell
    for (auto & i : table) {
        for (int j = 0; j < i.size(); ++j) {
            if(max_elem[j] < i[j].size()){
                max_elem[j] = (int)i[j].size();
            }
        }
    }
    int width;
    int remain;
    for (int i = (int)table.size() - 1; i >= 0; --i) {
        // Find width
        for (int j = 0 ; j < table[i].size(); ++j) {
            width = 5 + 3 * max_elem[j] - 2;
            remain = width;
            std::cout << "| {";
            remain -= 2;
            for(Variable* v : table[i][j]){
                std::cout << *v;
                remain--;
                if (v != *table[i][j].rbegin()) {
                    std::cout << ", ";
                    remain -= 2;
                }
            }
            std::cout << "}";
            remain--;
            for (int k = 0; k < remain; ++k) {
                std::cout << " ";
            }
        }
        std::cout << "|" << std::endl;
    }

    if(std::find(table[input.length() - 1][0].begin() , table[input.length() - 1][0].end() , startingVar) != table[input.length() - 1][0].end()){
        res = true;
    }
    std::cout << std::boolalpha << res << std::endl;
}

void CFG::ll() {
    // Initialize containers
    std::map<Variable* , std::set<Variable*>> firstVars;
    std::map<Variable* , std::set<Variable*>> followVars;
    std::cout << ">>> Building LL(1) Table" << std::endl;
    std::cout << " >> FIRST:" << std::endl;
    // Calculate first and follow sets
    bool eval = true;
    for(auto v : variables){
        firstVars[v] = v->calculateFirst();
    }
    for(auto v : firstVars){
        v.first->setFirstVar(v.second);
    }
    while(eval){
        eval = false;
        for(auto v : variables){
            int oldSize = (int)v->getFollowVar().size();
            for(const auto& p : v->getProductions()){
                Variable::follow(v , p);
            }
            if(oldSize != (int)v->getFollowVar().size()){
                eval = true;
            }
        }
    }
    std::vector<Variable*> firstVars_vec;
    std::vector<Variable*> followVars_vec;

    for(auto v : variables){
        followVars[v] = v->getFollowVar();
    }
    // Print first and follow sets
    for(auto v : variables){
        firstVars_vec = {firstVars[v].begin() , firstVars[v].end()};
        std::sort(firstVars_vec.begin() , firstVars_vec.end() , compareVariables);
        std::cout << "    " << *v << ": {";
        // Print first sets
        for(auto f : firstVars_vec){
            std::cout << *f;
            if(f != *firstVars_vec.rbegin()){
                std::cout << ", ";
            }
        }
        std::cout << "}" << std::endl;
    }
    std::cout << " >> FOLLOW:" << std::endl;
    for(auto v : variables){
        followVars_vec = {followVars[v].begin() , followVars[v].end()};
        std::sort(followVars_vec.begin() , followVars_vec.end() , compareVariables);
        std::cout << "    " << *v << ": {";
        // Print first sets
        for(auto f : followVars_vec){
            if(f->getName().empty()){
                continue;
            }
            std::cout << *f;
            if(f != *followVars_vec.rbegin()){
                std::cout << ", ";
            }
        }
        std::cout << "}" << std::endl;
    }
    std::cout << ">>> Table is built.\n"
                 "\n"
                 "-------------------------------------"
                 "\n"
                 << std::endl;
    // Get the max column width
    std::map<Variable* , int> width;
    std::map< Variable* , std::vector< std::pair<Variable* , std::vector<Variable*> > > > elements;
    for(auto v : variables){
        elements[v] = std::vector< std::pair<Variable* , std::vector<Variable*> > >(terminals.size() + 1 ,
                                                         std::pair<Variable* , std::vector<Variable*> >(nullptr , {})
                                                         );
    }
    int varMax = 0;
    int varWidth;
    for(auto v : variables){
        if (v->getName().size() > varMax) varMax = (int) v->getName().size();
    }
    varWidth = varMax + 3;
    for(auto t : terminals){
        for(auto v : variables){
            for(auto p : v->getProductions()){
                if(p.empty()){
                    continue;
                }
                if(p[0] == t && width[t] < (int) Variable::getProduction(p).size() + 3) {
                    width[t] = (int)Variable::getProduction(p).size() + 3;
                }
            }
        }
    }
    // Get all table elements
    for(auto v : variables){
        auto firstVec = v->getFirstVar();
        auto followVec = v->getFollowVar();
        for(int i = 0; i < terminals.size(); i++){
            // If neither -> put <ERR> in (leave it empty)
            if(firstVec.find(terminals[i]) == firstVec.end() && followVec.find(terminals[i]) == followVec.end()){
                elements[v][i] = std::pair<Variable* , std::vector<Variable*>>(terminals[i] , {});
            }
            for(const auto& p : v->getProductions()){
                if(p.empty()){
                    continue;
                }
                // If t in first set -> put production in
                if(firstVec.find(terminals[i]) != firstVec.end() && p[0] == terminals[i]){
                    elements[v][i] = std::pair<Variable* , std::vector<Variable*>>(terminals[i] , p);
                }
                // If t in follow set -> put epsilon production in
                if(followVec.find(terminals[i]) != followVec.end() && p[0]->getName().empty()){
                    elements[v][i] = std::pair<Variable* , std::vector<Variable*>>(terminals[i] , p);
                }
            }
        }
        // For <EOS>
        auto endVar = std::find_if(followVec.begin() , followVec.end() , [&](Variable* v1){return v1->getName() == "<EOS>";});
        auto endVar2 = std::find_if(firstVec.begin() , firstVec.end() , [&](Variable* v1){return v1->getName().empty();});
        if(endVar != followVec.end() && endVar2 != firstVec.end()){
            elements[v].back() = std::pair<Variable* , std::vector<Variable*>>(*endVar , {*endVar});
        }
    }
    // Recheck size
    for(auto t : terminals){
        for(auto v : variables){
            auto firstVec = v->getFirstVar();
            auto followVec = v->getFollowVar();
            auto endVar = std::find_if(followVec.begin() , followVec.end() , [&](Variable* v1){return v1->getName() == "<EOS>";});
            auto endVar2 = std::find_if(firstVec.begin() , firstVec.end() , [&](Variable* v1){return v1->getName().empty();});
            if(width[t] == 0 && (endVar != followVec.end() || endVar2 != firstVec.end())){
                width[t] = 3 + (int)t->getName().size();
            }
        }
        if(width[t] == 0){
            width[t] = 8;
        }
    }
    // Print table
    // Lookahead symbols row
    std::cout << std::string(varWidth + 1 , ' ');
    std::cout << "|";
    for(auto p : width){
        int k = p.second;
        std::cout << " ";
        std::cout << p.first->getName();
        k -= (int)p.first->getName().size() + 1;
        std::cout << std::string(k , ' ');
        std::cout << "|";
    }
    std::cout << " <EOS>  |" << std::endl;
    std::cout << "|" << std::string(varWidth , '-') << "|";
    for(auto p : width){
        std::cout << std::string(p.second , '-') << "|";
    }
    std::cout << "--------|" << std::endl;
    // Print table cells
    for(auto v : variables){
        std::cout << "| " << v->getName() << std::string(varWidth - v->getName().size() - 1 , ' ') << "|";
        // Loop through the columns
        auto vec = elements[v];
        for(auto s : width){
            auto sym = s.first;
            int k = s.second;
            std::cout << " ";
            k -= 1;
            for(auto p : vec){
                if(p.first == sym){
                    if(!p.second.empty()){
                        if(!p.second[0]->getName().empty()){
                            std::string cell = Variable::getProduction(p.second);
                            std::cout << cell;
                            k -= (int)cell.size();
                        }
                    }
                    else{
                        std::cout << "<ERR>";
                        k -= 5;
                    }
                    std::cout << std::string(k , ' ') << "|";
                    break;
                }
            }
        }
        if(vec.back().first == nullptr && vec.back().second.empty()){
            std::cout << " <ERR>  |";
        }
        else{
            std::cout << "        |";
        }

        std::cout << std::endl;
    }
    std::cout << "|" << std::string(varWidth , '-') << "|";
    for(auto p : width){
        std::cout << std::string(p.second , '-') << "|";
    }
    std::cout << "--------|" << std::endl;
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
    }
    current += "}";
    std::cout << current << std::endl;
    // Print terminals
    current = "T = {";
    for(const auto& t : terminals){
        current += t->getName();
        if(t != *terminals.rbegin()){
            current += ", ";
        }
    }
    current += "}";
    std::cout << current << std::endl;
    // Print productions
    sortProductions();
    current = "P = {";
    current += '\n';
    for(auto v : variables){
        // Get productions in a vector
        std::vector<std::string> prod;
        for(auto p : v->getProductions()){
            prod.push_back(Variable::getProduction(p));
        }
//        std::sort(prod.begin() , prod.end());
        for(const auto& s : prod){
            current += "  " + v->getName() + " -> " + s + " \n";
        }
        prod.clear();
    }
    current += '}';
    current += '\n';
    std::cout << current << "S = ";
    if(startingVar != nullptr){
         std::cout << startingVar->getName() << std::endl;
    }
}

CFG::~CFG() {
    for(auto v : variables){
        for(auto & it : v->getProductions()){
            for(auto & it1 : it){
                if(it1->getName().empty()){
                    std::cout << "Deleting " << *it1 << std::endl;
                    delete it1;
                }
            }
        }
        for(auto it : v->getFollowVar()){
            if(it->getName() == "<EOS>"){
                std::cout << "Deleting " << *it << std::endl;
                delete it;
            }
        }
    }
    for(auto v : variables){
        delete v;
    }
    for(auto t : terminals){
        delete t;
    }
    terminals.clear();
    variables.clear();
}
