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
    std::set<std::pair<Variable* , Variable*>> calculateUnits();
    // Useless variables elimination
    void eliminateUseless();
    std::vector<Variable*> calculateGenerating();
    void eliminateNonGenerating();
    std::vector<Variable*> calculateReachable();
    void eliminateUnreachable(const std::vector<Variable*>& reachVars);
    void fixTerminals();
    void fixVariables();
    // Internal functions for LL(1) parser


public:

    /**
     * @brief Constructor for Context Free Grammar object
     * @param filename The path to the input file
     */
    explicit CFG (const std::string& filename);

    /**
     * @brief Creates a Context Free Grammar object with default parameters
     */
    CFG();

    // Getters and setters

    const std::vector<Variable *> &getVariables() const;

    void setVariables(const std::vector<Variable *> &newVars);

    const std::vector<Variable *> &getTerminals() const;

    void setTerminals(const std::vector<Variable *> &newTerms);

    Variable *getStartingVar() const;

    void setStartingVar(Variable *newStartingVar);

    // CNF conversion

    /**
     * Converts the CFG to CNF while displaying all the steps
     */
    void toCNF();

    /**
     * Prints the CYK table and whether the input is accepted or not
     * @param input The input to check
     */
    void accepts(const std::string& input);

    /**
     * Constructs and prints the LL(1) table for the grammar
     */
    void ll();

    /**
     * Prints all the elements of the Context Free Grammar
     */
    void print();

    virtual ~CFG();

};

#endif //AUTOMATA_CFG_H
