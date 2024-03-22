#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "Level.h"
#include "Player.h"
#include "Bomb.h"
#include "Bonus.h"
#include "Flame.h"

enum Winner
{
    PLAYER1,
    PLAYER2,
    DRAW,
    NO_WINNER
};

class GameState
{
public:
    Level level;
    Player players[2];
    bool AIturn;
    std::vector<Bomb> bombs;
    std::vector<Bonus> bonuses;
    std::vector<Flame> flames;

    Winner winner;
    int turns;
};

#endif // GAMESTATE_H