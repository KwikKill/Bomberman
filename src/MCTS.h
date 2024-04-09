#ifndef MCST_H
#define MCST_H

#include "Player.h"
#include "GameState.h"
#include "Node.h"

#define NUM_SIMULATIONS 1000
#define MAX_TURNS 500

class MCTS {
private:
    Node* root;
public:
    MCTS() {
        root = nullptr;
    };
    void init(GameState& currentState);
    Action findBestAction();
    void nextSimulation(Action action);

    bool isInitialized() const { return root != nullptr; }
};

#endif // MCST_H