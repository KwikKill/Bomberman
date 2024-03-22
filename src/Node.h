#include "GameState.h"

#ifndef NODE_H
#define NODE_H

#define NUM_SIMULATIONS 1000
#define MAX_TURNS 100

enum Action {
    NO_ACTION,
    MOVE_UP,
    MOVE_DOWN,
    MOVE_LEFT,
    MOVE_RIGHT,
    PLACE_BOMB
};

class Node {
public:
    GameState state;
    Action actionTaken;
    Node* parent;
    std::vector<Node*> children;
    int wins;
    int visits;

    Node(GameState state, Action actionTaken, Node* parent)
        : state(state), actionTaken(actionTaken), parent(parent), wins(0), visits(0) {}
};

#endif