#ifndef AUTOMATA_RE_H
#define AUTOMATA_RE_H

#include "ENFA.h"
#include <vector>
class RE {
private:
    std::string regex;
    char eps;
public:

    // Constructor

    /**
     * @brief Construct a new RE object
     * 
     * @param regex The regex std::string
     * @param eps The character for the epsilon
     */
    RE(const std::string &regex, char eps);

    /**
     * @brief Construct a new RE object from a reference RE object
     * 
     * @param refRE The reference object
     */
    explicit RE(RE* refRE);

    /**
     * @brief Construct a new RE object
     * @details The regex and epsilon characters are initialized as empty
     */
    RE();

    // Getters and setters

    /**
     * @brief Get the Regex std::string
     * 
     * @return const std::string& The regex std::string
     */
    const std::string &getRegex() const;

    /**
     * @brief Set the Regex std::string
     * 
     * @param newRegex The new newRegex std::string
     */
    void setRegex(const std::string &newRegex);

    /**
     * @brief Get the Eps character
     * 
     * @return char The epsilon character
     */
    char getEps() const;

    /**
     * @brief Set the Eps character
     * 
     * @param newEps The new epsilon character
     */
    void setEps(char newEps);

    // Basic automatons

    /**
     * @brief Create an enfa for epsilon language {eps}
     * 
     * @param beginName Name of the beginState state
     * @param endName Name of the endState state
     * @return ENFA The epsilon enfa
     */
    ENFA* createEpsilon(std::string beginName , std::string endName) const;

    /**
     * @brief Create an enfa for the empty language {}
     * 
     * @param beginName Name of the beginState state
     * @param endName Name of the endState state
     * @return ENFA The empty enfa
     */
    ENFA* createEmpty(std::string beginName , std::string endName) const;

    /**
     * @brief Create an enfa for the language {a}
     * 
     * @param beginName Name of the beginState state
     * @param endName Name of the endState state
     * @param a The character
     * @return ENFA The enfa for the language {a}
     */
    ENFA* createSingleChar(std::string beginName , std::string endName , char a) const;

    // Induction automatons

    /**
     * @brief Create an enfa for the plus operation (R + S)
     * 
     * @param beginName Name of the beginState state
     * @param endName Name of the endState state
     * @param ref The std::vector of the enfa's to plus
     * @return ENFA The enfa for the operation R + S
     */
    ENFA* createPlus(std::vector<ENFA*> &ref , int &count);

    /**
     * @brief Create an enfa for the concatenation operation (R.S)
     * 
     * @param R The first enfa
     * @param S The second enfa
     * @return ENFA The enfa for the operation R.S
     */
    ENFA* createConcatenation(ENFA &R , ENFA &S) const;

    /**
     * @brief Create an enfa for the concatenation of a series of ENFA's
     * 
     * @param ref The std::vector of the enfa's
     * @return ENFA* A pointer to the concatenated ENFA
     */
    ENFA* createConcatenation(std::vector<ENFA*> ref) const;
    
    /**
     * @brief Create an enfa for the star operation (R*)
     * 
     * @param beginName Name of the beginState state
     * @param endName Name of the endState state
     * @param R The base enfa
     * @return ENFA The enfa for the operation R*
     */
    ENFA* createStar(std::string beginName , std::string endName , ENFA &R) const;

    // Convert function

    /**
     * @brief Splits the regex std::string into substrings
     * 
     * @param reg The REGEX
     * @return std::vector<std::string> A std::vector of substrings
     */
    std::vector<ENFA*> splitRegex(std::string &reg , int &count , std::vector<int>&index);

    /**
     * @brief Creates an ENFA from a regex
     * 
     * @param reg The RE std::string
     * @param count The current name count
     * @return ENFA* A pointer to the created ENFA
     */
    ENFA* toENFA(std::string &reg , int &count) const;

    /**
     * @brief Constructs an enfa from the regex
     * 
     * @return ENFA The enfa of the regex
     */
    ENFA toENFA();

    // Destructor
    virtual ~RE();
};


#endif //AUTOMATA_RE_H
