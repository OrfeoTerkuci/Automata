#pragma once
#ifndef AUTOMATA_NODE_H
#define AUTOMATA_NODE_H
#include <iostream>
#include <vector>

class Node
{
private:
    std::string name;
    bool starting;
    bool accepting;
public:
    Node(std::string name, bool starting, bool accepting);
    explicit Node(Node* refNode);
    Node();
    void setStarting(bool newStarting);
    bool isStarting()const;
    void setAccepting(bool newAccepting);
    bool isAccepting()const;
    void setName(std::string newName);
    std::string getName()const;
    ~Node();
};

#endif //AUTOMATA_NODE_H