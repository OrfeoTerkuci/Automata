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
    std::map<std::string , std::vector<std::vector< std::string> > > productions;
    Variable* startingVar;

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

    /**
     * Prints all the elements of the Context Free Grammar
     */
    void print();

    virtual ~CFG();

};

#endif //AUTOMATA_CFG_H
