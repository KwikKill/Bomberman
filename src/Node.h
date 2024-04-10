#include "GameState.h"

#ifndef NODE_H
#define NODE_H

class Node {
public:
    GameState state;
    Action actionTaken;
    bool isFullyExpanded;
    Node* parent;
    std::vector<Node*> children;
    int wins;
    int visits;

    Node(GameState state, Action actionTaken, Node* parent)
        : state(state), actionTaken(actionTaken), parent(parent), wins(0), visits(0), isFullyExpanded(false) {}

    Node(Action actionTaken, GameState& state, Node* parent)
        : state(state), actionTaken(actionTaken), parent(parent), wins(0), visits(0), isFullyExpanded(false) {}
};

#endif