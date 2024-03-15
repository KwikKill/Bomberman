#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include "Level.h"
#include "Player.h"
#include "Bomb.h"
#include "Bonus.h"

#define TILE_SIZE 64

#define BONUS_SPAWN_CHANCE 15

enum Winner
{
    PLAYER1,
    PLAYER2,
    DRAW,
    NO_WINNER
};

class Game
{
private:
    void processEvents();
    void update();
    void render();

    sf::RenderWindow window; 
    Level level;

    // Array of players
    Player players[2];

    // Use to keep track of the player movement state
    bool hasMoved;

    bool isLegalMove(int x, int y);

    // Array of bombs
    std::vector<Bomb> bombs;

    // Array of bonuses
    std::vector<Bonus> bonuses;

    void PlayerCheckBonus(Player &player);

    Winner winner;

public:
    Game();
    void load(unsigned int level_nb);
    void run();
};

#endif // GAME_H