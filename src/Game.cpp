#include <SFML/Graphics.hpp>
#include <iostream>
#include "Level.h"
#include "Game.h"
#include "Player.h"
#include "Bomb.h"
#include "Bonus.h"
#include "HUD.h"
#include <optional>
#include <algorithm>

Game::Game() {
    gameState.AIturn = false;
    gameState.winner = NO_WINNER;
    gameState.level = Level();
    gameState.turns = 0;
    zoom = 1.0;
}

void Game::load(unsigned int level_nb) {
    // Create a new level
    gameState.level.load(level_nb);

    zoom = 1.0;

    int X_size = gameState.level.getWidth() * TILE_SIZE;
    int Y_size = gameState.level.getHeight() * TILE_SIZE;
    while (X_size > MAX_WINDOW_X * TILE_SIZE || Y_size > MAX_WINDOW_Y * TILE_SIZE) {
        X_size /= 2;
        Y_size /= 2;
        zoom=zoom*2.0;
    }

    // Create a window of the right size
    window.create(
        sf::VideoMode(
            X_size,
            Y_size + HUD_HEIGHT
        ),
        "Bomberman",
        sf::Style::Titlebar | sf::Style::Close
    );
    window.setFramerateLimit(60); // Limit the frame rate to 60 frames per second

    // Randomly choose a spawn position for the player
    std::vector<std::pair<int, int>> spawnPositions = gameState.level.getSpawnPositions();
    int randomIndex = rand() % spawnPositions.size();

    // Create the players and set their positions
    gameState.players[0] = Player(
        spawnPositions[randomIndex].first,
        spawnPositions[randomIndex].second,
        DEFAULT_PLAYER_SPEED,
        "assets/img/player.png",
        PLAYER,
        zoom
    );
    gameState.players[1] = Player(
        spawnPositions[(randomIndex + 1) % spawnPositions.size()].first,
        spawnPositions[(randomIndex + 1) % spawnPositions.size()].second,
        DEFAULT_PLAYER_SPEED,
        "assets/img/ai.png",
        AI,
        zoom
    );
}

void Game::run()
{
    while (window.isOpen())
    {
        if (gameState.winner == NO_WINNER) {
            gameState.AIturn = false;
            processEvents();

            window.clear(sf::Color::Black); // Clear the window with black color

            render();
            if (gameState.AIturn) {
                update();
            }

            window.display(); // End the current frame and display everything
        } else {
            std::cout << "The winner is: ";
            if (gameState.winner == PLAYER1) {
                std::cout << "Player 1" << std::endl;
            } else if (gameState.winner == PLAYER2) {
                std::cout << "Player 2" << std::endl;
            } else {
                std::cout << "Nobody" << std::endl;
            }
            window.close();
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
            // print player strength and number of bombs
            if (event.key.code == sf::Keyboard::Up) {
                gameState.players[0].play(MOVE_UP, gameState);
                gameState.AIturn = true;
            } else if (event.key.code == sf::Keyboard::Down) {
                gameState.players[0].play(MOVE_DOWN, gameState);
                gameState.AIturn = true;
            } else if (event.key.code == sf::Keyboard::Left) {
                gameState.players[0].play(MOVE_LEFT, gameState);
                gameState.AIturn = true;
            } else if (event.key.code == sf::Keyboard::Right) {
                gameState.players[0].play(MOVE_RIGHT, gameState);
                gameState.AIturn = true;
            } else if (event.key.code == sf::Keyboard::Space) {
                gameState.players[0].play(PLACE_BOMB, gameState);
                gameState.AIturn = true;
            }
        }
    }
}

void Game::update()
{
    // Update the players
    for (int i = 0; i < 2; ++i) {
        gameState.players[i].update(gameState);
    }

    // Update the game state
    gameState.update();
}

void Game::render()
{
    // Draw the current game state
    gameState.level.draw(window, zoom);
    
    // Draw the players
    for (int i = 0; i < 2; ++i) {
        gameState.players[i].draw(window, zoom);
    }

    // Draw the bonuses
    for (long unsigned i = 0; i < gameState.bonuses.size(); ++i) {
        gameState.bonuses[i].draw(window, zoom);
    }

    // Draw the bombs
    for (long unsigned i = 0; i < gameState.bombs.size(); ++i) {
        gameState.bombs[i].draw(window, zoom);
    }

    // Draw the flames
    for (long unsigned i = 0; i < gameState.flames.size(); ++i) {
        gameState.flames[i].draw(window, zoom);
    }

    // Draw the HUD
    HUD::draw(window, *this);
}