#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <optional>
#include "GameState.h"

#define TILE_SIZE 64
#define MAX_WINDOW_X 30
#define MAX_WINDOW_Y 10

#define BONUS_SPAWN_CHANCE 15

class Game
{
private:
    void processEvents();
    void update();
    void render();

    sf::RenderWindow window; 

    // GameState
    GameState gameState;

    float zoom;

public:
    Game();
    void load(unsigned int level_nb);
    void run();

    Level &getLevel() { return gameState.level; }
    std::vector<Player> &getPlayers() { return gameState.players; }
    std::vector<Bomb> &getBombs() { return gameState.bombs; }
};

#endif // GAME_H