#pragma once
#ifndef AUTOMATA_NODE_H
#define AUTOMATA_NODE_H
#include <iostream>
#include <vector>

class transition;
class Node
{
private:
    std::string name;
    bool starting;
    bool accepting;
public:
    Node(std::string name, bool starting, bool accepting);
    Node(Node* refNode);
    Node();
    void setStarting(bool starting);
    bool isStarting()const;
    void setAccepting(bool accepting);
    bool isAccepting()const;
    void setName(std::string name);
    std::string getName()const;
    ~Node();
};

#endif //AUTOMATA_NODE_H