#include <SFML/Graphics.hpp>
#include <iostream>
#include "Level.h"
#include "Game.h"
#include "Player.h"
#include "Bomb.h"

void Game::load() {
    // Create a new level
    level.load(1);

    // Create a window of the right size
    window.create(
        sf::VideoMode(
            level.getWidth()*TILE_SIZE,
            level.getHeight()*TILE_SIZE
        ),
        "Bomberman",
        sf::Style::Titlebar | sf::Style::Close
    );
    window.setFramerateLimit(60); // Limit the frame rate to 60 frames per second

    // Randomly choose a spawn position for the player
    std::vector<std::pair<int, int>> spawnPositions = level.getSpawnPositions();
    int randomIndex = rand() % spawnPositions.size();

    // Create the players and set their positions
    players[0] = Player(
        spawnPositions[randomIndex].first,
        spawnPositions[randomIndex].second,
        DEFAULT_PLAYER_SPEED,
        "assets/img/player.png",
        PLAYER
    );
    players[1] = Player(
        spawnPositions[(randomIndex + 1) % spawnPositions.size()].first,
        spawnPositions[(randomIndex + 1) % spawnPositions.size()].second,
        DEFAULT_PLAYER_SPEED,
        "assets/img/ai.png",
        AI
    );

    numBombs = 0;
}

void Game::run()
{
    while (window.isOpen())
    {
        hasMoved = false;
        processEvents();

        window.clear(sf::Color::Black); // Clear the window with black color

        render();
        if (hasMoved) {
            update();
        }

        window.display(); // End the current frame and display everything
    }
}

void Game::processEvents()
{
    sf::Event event;
    while (window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
            window.close();
        else if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Up && level.isLegalMove(players[0].getX(), players[0].getY() - 1)) {
                players[0].move(0, -1);
                hasMoved = true;
            } else if (event.key.code == sf::Keyboard::Down && level.isLegalMove(players[0].getX(), players[0].getY() + 1)) {
                players[0].move(0, 1);
                hasMoved = true;
            } else if (event.key.code == sf::Keyboard::Left && level.isLegalMove(players[0].getX() - 1, players[0].getY())) {
                players[0].move(-1, 0);
                hasMoved = true;
            } else if (event.key.code == sf::Keyboard::Right && level.isLegalMove(players[0].getX() + 1, players[0].getY())) {
                players[0].move(1, 0);
                hasMoved = true;
            } else if (event.key.code == sf::Keyboard::Space) {
                if(players[0].dropBomb()) {
                    // Create a new bomb
                    bombs[numBombs] = Bomb(
                        players[0].getX(),
                        players[0].getY(),
                        DEFAULT_BOMB_TIMER,
                        players[0].getStrength(),
                        "assets/img/bomb1.png"
                    );
                    numBombs++;
                }
            }
        }
    }
}

void Game::update()
{
    for (int i = 0; i < 2; ++i) {
        players[i].update();
    }
}

void Game::render()
{
    // Draw the current game state
    level.draw(window);
    
    // Draw the players
    for (int i = 0; i < 2; ++i) {
        players[i].draw(window);
    }

    // Draw the bombs
    for (int i = 0; i < numBombs; ++i) {
        bombs[i].draw(window);
    }
}