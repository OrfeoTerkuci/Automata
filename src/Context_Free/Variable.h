#ifndef AUTOMATA_VARIABLE_H
#define AUTOMATA_VARIABLE_H

#include <iostream>
#include <vector>
#include <set>

class Variable {

private:
    std::string name;
    std::vector<std::vector<Variable*> > production;
    bool starting {false};
    bool terminal {false};
    bool nullable {true};
    bool generating {false};
    bool reachable {false};

    std::set<Variable*> firstVar;
    std::set<Variable*> followVar;

    std::set<Variable*> first(const std::vector<Variable*>& prod);

public:
    explicit Variable(const std::string& name, std::vector<std::vector<Variable*> > production = {},
                      bool starting = false , bool terminal = false , bool generating = false);

    Variable() = default;

    const std::string &getName() const { return name; }

    void setName(const std::string &newName) { name = newName; }

    std::vector<std::vector<Variable*> > getProductions() const { return production; }

    void setProductions(const std::vector<std::vector<Variable*> > &newProductions) { production = newProductions; }

    void addProduction(std::vector<Variable*> newProduction);

    void removeProduction(std::vector<Variable*> &prod);

    void replaceProduction(std::vector<Variable*> &prod , std::vector<std::vector<Variable*>> &newProd);

    void addProduction(const std::vector<std::string>& newProduction , std::vector<Variable*> &vars , std::vector<Variable*> &terms);

    bool isStarting() const { return starting; }

    void setStarting(bool newStarting) { starting = newStarting; }

    bool isTerminal() const { return terminal; }

    void setTerminal(bool newTerminal) {terminal = newTerminal; }

    bool isReachable() const { return reachable; }

    void setReachable(bool newReachableStatus) { Variable::reachable = newReachableStatus; }

    static std::string getProduction(std::vector<std::string> &prod);

    static std::string getProduction(std::vector<Variable*> &prod);

    bool hasProduction(const char &t) const;

    bool hasProduction(const std::string &t) const;

    bool hasProduction(const std::vector<Variable*>& p) const;

    // Nullable check

    bool isNullable() const { return nullable; }

    void setNullable(bool newNullStat) { nullable = newNullStat; }

    bool isNullVar();

    // Unit check

    std::pair<bool , std::set<Variable*> > isUnit();

    bool prodExists(std::vector<Variable*> &newProduction);

    // Generating check

    bool isGenerating() const { return generating; }

    void setGenerating(bool newGenerating) { generating = newGenerating; }

    bool isGeneratingVar();

    void eliminateNonGen();

    void eliminateNonReach();

    // FIRST and FOLLOW sets
    std::set<Variable*> calculateFirst();

    static void follow(Variable* var , const std::vector<Variable*>& prod);

    const std::set<Variable *> &getFollowVar() const { return followVar; }

    void setFollowVar(const std::set<Variable *> &newFollowSet) { followVar = newFollowSet; }

    const std::set<Variable *> &getFirstVar() const { return firstVar; }

    void setFirstVar(const std::set<Variable *> &newFirstSet) { firstVar = newFirstSet; }

    // Operator overloads

    bool operator==(const Variable &rhs) const;

    bool operator!=(const Variable &rhs) const;

    bool operator<(const Variable &rhs) const;

    bool operator>(const Variable &rhs) const;

    bool operator<=(const Variable &rhs) const;

    bool operator>=(const Variable &rhs) const;

    friend std::ostream &operator<<(std::ostream &os, const Variable &variable);

    virtual ~Variable() = default;

};


#endif //AUTOMATA_VARIABLE_H
