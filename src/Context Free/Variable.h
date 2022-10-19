#ifndef AUTOMATA_VARIABLE_H
#define AUTOMATA_VARIABLE_H

#include <iostream>
#include <vector>
#include <set>

class Variable {

private:
    std::string name;
    std::vector<std::vector<Variable*> > production;
    bool starting;
    bool terminal;
    bool nullable;
public:
    Variable(const std::string &name, const std::vector<std::vector<Variable*> > &production = {},
                      bool starting = false , bool terminal = false);

    Variable();

    const std::string &getName() const;

    void setName(const std::string &newName);

    std::vector<std::vector<Variable*> > getProductions() const;

    void setProductions(const std::vector<std::vector<Variable*> > &newProductions);

    void addProduction(std::vector<Variable*> newProduction);

    void addProduction(std::vector<std::string> newProduction , std::vector<Variable*> &vars , std::vector<Variable*> &terms);

    bool isStarting() const;

    void setStarting(bool newStarting);

    bool isTerminal() const;

    void setTerminal(bool newTerminal);

    static std::string getProduction(std::vector<std::string> &prod);

    static std::string getProduction(std::vector<Variable*> &prod);

    bool isNullable() const;

    void setNullable(bool newNullStat);

    bool isNullVar();

    // Operator overloads

    bool operator==(const Variable &rhs) const;

    bool operator!=(const Variable &rhs) const;

    bool operator<(const Variable &rhs) const;

    bool operator>(const Variable &rhs) const;

    bool operator<=(const Variable &rhs) const;

    bool operator>=(const Variable &rhs) const;

    virtual ~Variable();

};


#endif //AUTOMATA_VARIABLE_H
