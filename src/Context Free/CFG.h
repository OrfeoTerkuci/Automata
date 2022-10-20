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

    // Internal functions for CNF conversion
    void eliminateEpsilon();
    std::vector<Variable*> calculateNullables();
    void eliminateUnitPairs();
    void eliminateUnitProductions();
    void sortProductions();
    std::set<std::pair<Variable* , Variable*>> calculateUnits(int &total);
    static std::set<std::vector<Variable*>> getProductions(std::set<std::pair<Variable* , Variable*>> &unitPairs , Variable* var);
    void eliminateUseless();
    static void fixNullable(Variable* &var);
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
