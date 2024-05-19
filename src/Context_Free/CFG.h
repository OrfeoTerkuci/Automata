#ifndef AUTOMATA_CFG_H
#define AUTOMATA_CFG_H

#include <iostream>
#include <map>
#include <vector>
#include <set>

class Variable;

class FileExist : public std::exception {
private:
    std::string error {};
public:
    /**
     * Constructor for FileExists type exception
     * @param error The error message
     */
    explicit FileExist(std::string  error) : error(std::move(error)) {}

    const char* what() const noexcept override {
        return error.c_str();
    }
};

class FileEmpty : public std::exception {
private:
    std::string error {};
public:
    /**
     * Constructor for FileEmpty type exception
     * @param error The error message
     */
    explicit FileEmpty(std::string  error) : error(std::move(error)) {}

    /**
     * Copy constructor for FileEmpty type exception
     * @param ref Reference to another FileEmpty object
     */
    FileEmpty(const FileEmpty& ref) noexcept : error(ref.error) {}

    const char* what() const noexcept override {
        return error.c_str();
    }

    ~FileEmpty() override = default;
};

class CFG {

private:
    std::vector<Variable*> variables {};
    std::vector<Variable*> terminals {};
    Variable* startingVar {nullptr};

    std::string filename;

    void sortProductions();
    // Internal functions for CNF conversion
    // Epsilon elimination
    void eliminateEpsilon(bool printToggle = true);
    std::vector<Variable*> calculateNullables();
    static void fixNullable(Variable* &var);
    // Unit pair elimination
    void eliminateUnitPairs(bool printToggle = true);
    void eliminateUnitProductions();
    std::set<std::pair<Variable* , Variable*>> calculateUnits();
    // Useless variables elimination
    void eliminateUseless(bool printToggle = true);
    std::vector<Variable*> calculateGenerating();
    void eliminateNonGenerating();
    std::vector<Variable*> calculateReachable();
    std::vector<Variable*> calculateReachable(std::set<Variable*>& reachVar);
    void eliminateUnreachable(const std::vector<Variable*>& reachVars);
    void fixTerminals(bool printToggle = true);
    void fixVariables(bool printToggle = true);


public:

    /**
     * @brief Constructor for Context Free Grammar object
     * @param filename The path to the input file
     */
    explicit CFG (const std::string& filename);

    /**
     * @brief Creates a Context Free Grammar object with default parameters
     */
    CFG() = default;

    // Getters and setters

    const std::vector<Variable *> &getVariables() const { return variables; }

    void setVariables(const std::vector<Variable *> &newVars) { variables = newVars; }

    const std::vector<Variable *> &getTerminals() const { return terminals; }

    void setTerminals(const std::vector<Variable *> &newTerms) { terminals = newTerms; }

    Variable *getStartingVar() const { return startingVar; }

    void setStartingVar(Variable *newStartingVar) { startingVar = newStartingVar; }

    const std::string &getFilename() const { return filename; }

    void setFilename(const std::string &newFilename) { CFG::filename = newFilename; }

    // CNF conversion

    /**
     * Converts the CFG to CNF while displaying all the steps
     */
    void toCNF(bool printToggle = true);

    /**
     * Prints the CYK table and whether the input is accepted or not
     * @param input The input to check
     */
    bool accepts(const std::string& input , bool printToggle = true);

    /**
     * Constructs and prints the LL(1) table for the grammar
     */
    void ll();

    /**
     * Prints all the elements of the Context Free Grammar
     */
    void print();

    /**
     * @brief Saves the CFG as a json file following the filepath it was provided
     */
    void save();

    /**
     * @brief Split algorithm for creating new non-terminal productions
     */
    void split();

    /**
     * @brief Trains a variable with terminal productions from samples in the given filename
     * @param variable_name The name of the variable to train
     * @param t_filename The path to the file containing the training samples
     */
    void trainTerminals(const std::string &variable_name, const std::string &t_filename);

    /**
     * @brief Same functionality as the trainTerminals function, but
     * @param variable_name
     * @param begin_num
     * @param end_num
     * @param t_filename
     */
    void trainInterval(const std::string& variable_name , int begin_num , int end_num , const std::string& t_filename);

    virtual ~CFG();

};

#endif //AUTOMATA_CFG_H
