#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <optional>
#include "Level.h"
#include "Player.h"
#include "Bomb.h"
#include "Bonus.h"
#include "Flame.h"

#define TILE_SIZE 64
#define MAX_WINDOW_X 30
#define MAX_WINDOW_Y 10

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

    // Array of bombs
    std::vector<Bomb> bombs;

    // Array of bonuses
    std::vector<Bonus> bonuses;

    // Array of flames
    std::vector<Flame> flames;

    void PlayerCheckBonus(Player &player);

    Winner winner;

    float zoom;

public:
    Game();
    void load(unsigned int level_nb);
    void run();

    Level &getLevel() { return level; }
    Player *getPlayers() { return players; }
    std::vector<Bomb> &getBombs() { return bombs; }

    bool isLegalMove(int x, int y, std::optional<Player> player = std::nullopt);
};

#endif // GAME_H