#ifndef MCST_H
#define MCST_H

#include "Player.h"
#include "GameState.h"

class MCTS {
public:
    Action findBestAction(GameState currentState);
};

#endif // MCST_H