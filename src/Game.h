#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include "Level.h"
#include "Player.h"
#include "Bomb.h"

#define TILE_SIZE 64
#define MAX_BOMBS 100

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
    Bomb bombs[MAX_BOMBS];
    int numBombs;

public:
    void load();
    void run();
};

#endif // GAME_H