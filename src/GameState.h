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
    GameState() {}
    GameState(Level level, std::vector<Player> players, bool AIturn)
        : level(level), players(players), AIturn(AIturn), winner(NO_WINNER), turns(0) {}

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