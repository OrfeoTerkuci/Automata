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
    bool generating;
public:
    Variable(const std::string &name, const std::vector<std::vector<Variable*> > &production = {},
                      bool starting = false , bool terminal = false , bool generating = false);

    Variable();

    const std::string &getName() const;

    void setName(const std::string &newName);

    std::vector<std::vector<Variable*> > getProductions() const;

    void setProductions(const std::vector<std::vector<Variable*> > &newProductions);

    void addProduction(std::vector<Variable*> newProduction);

    void removeProduction(std::vector<Variable*> &prod);

    void replaceProduction(std::vector<Variable*> &prod , std::vector<std::vector<Variable*>> &newProd);

    void addProduction(const std::vector<std::string>& newProduction , std::vector<Variable*> &vars , std::vector<Variable*> &terms);

    bool isStarting() const;

    void setStarting(bool newStarting);

    bool isTerminal() const;

    void setTerminal(bool newTerminal);

    static std::string getProduction(std::vector<std::string> &prod);

    static std::string getProduction(std::vector<Variable*> &prod);

    bool hasProduction(const char &t) const;

    bool hasProduction(const std::vector<Variable*>& p) const;

    // Nullable check

    bool isNullable() const;

    void setNullable(bool newNullStat);

    bool isNullVar();

    // Unit check

    std::pair<bool , std::set<Variable*> > isUnit();

    bool prodExists(std::vector<Variable*> &newProduction);

    // Generating check

    bool isGenerating() const;

    void setGenerating(bool newGenerating);

    bool isGeneratingVar();

    void eliminateNonGen();

    // Operator overloads

    bool operator==(const Variable &rhs) const;

    bool operator!=(const Variable &rhs) const;

    bool operator<(const Variable &rhs) const;

    bool operator>(const Variable &rhs) const;

    bool operator<=(const Variable &rhs) const;

    bool operator>=(const Variable &rhs) const;

    friend std::ostream &operator<<(std::ostream &os, const Variable &variable);

    virtual ~Variable();

};


#endif //AUTOMATA_VARIABLE_H
