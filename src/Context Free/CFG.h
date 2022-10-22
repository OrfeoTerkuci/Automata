#ifndef AUTOMATA_CFG_H
#define AUTOMATA_CFG_H

#include <iostream>
#include <map>
#include <vector>
#include <set>

class Variable;

class CFG {

private:
    std::vector<Variable*> variables;
    std::vector<Variable*> terminals;
    Variable* startingVar;

    void sortProductions();
    // Internal functions for CNF conversion
    // Epsilon elimination
    void eliminateEpsilon();
    std::vector<Variable*> calculateNullables();
    static void fixNullable(Variable* &var);
    // Unit pair elimination
    void eliminateUnitPairs();
    void eliminateUnitProductions();
    std::set<std::pair<Variable* , Variable*>> calculateUnits(int &total);
    // Useless variables elimination
    void eliminateUseless();
    std::vector<Variable*> calculateGenerating();
    void eliminateNonGenerating();
    std::vector<Variable*> calculateReachable();
    void eliminateUnreachable(const std::vector<Variable*>& reachVars);
    void fixTerminals();
    void fixVariables();

public:

    /**
     * @brief Constructor for Context Free Grammar object
     * @param filename The path to the input file
     */
    explicit CFG (std::string filename);

    /**
     * @brief Creates a Context Free Grammar object with default parameters
     */
    CFG();

    // CNF conversion

    /**
     * Converts the CFG to CNF while displaying all the steps
     */
    void toCNF();

    /**
     * Prints all the elements of the Context Free Grammar
     */
    void print();

    virtual ~CFG();

};

#endif //AUTOMATA_CFG_H
