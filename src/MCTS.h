#ifndef MCST_H
#define MCST_H

#include "Player.h"
#include "GameState.h"

#define NUM_SIMULATIONS 1000
#define MAX_TURNS 500

class MCTS {
public:
    static Action findBestAction(GameState &currentState);
};

#endif // MCST_H