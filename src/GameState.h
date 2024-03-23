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
    std::vector<Player> players;
    bool AIturn;
    std::vector<Bomb> bombs;
    std::vector<Bonus> bonuses;
    std::vector<Flame> flames;

    Winner winner;
    int turns;

    void PlayerCheckBonus(Player &player);
    void update();
};

#endif // GAMESTATE_H