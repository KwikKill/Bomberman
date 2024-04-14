#include <SFML/Graphics.hpp>
#include <iostream>
#include <optional>
#include <algorithm>
#include <chrono>

#include "Level.h"
#include "Game.h"
#include "Player.h"
#include "Bomb.h"
#include "HUD.h"
#include "GameState.h"
#include "Flame.h"

void Game::load(unsigned int level_nb) {
    // Create a new level
    std::vector<Bomb> bombs = gameState.level.load(level_nb);

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
    //window.setFramerateLimit(60); // Limit the frame rate to 60 frames per second

    // Randomly choose a spawn position for the player
    std::vector<std::pair<int, int>> spawnPositions = gameState.level.getSpawnPositions();
    int SpawnPositionsSize = spawnPositions.size();
    int randomIndex = rand() % SpawnPositionsSize;

    // Create the players and set their positions
    gameState.players.push_back(Player(
        0,
        spawnPositions[randomIndex].first,
        spawnPositions[randomIndex].second,
        DEFAULT_PLAYER_SPEED,
        "assets/img/player.png",
        PLAYER,
        zoom
    ));
    gameState.players.push_back(Player(
        1,
        spawnPositions[(randomIndex + 1) % SpawnPositionsSize].first,
        spawnPositions[(randomIndex + 1) % SpawnPositionsSize].second,
        DEFAULT_PLAYER_SPEED,
        "assets/img/ai.png",
        AI,
        zoom
    ));
    gameState.bombs = bombs;

    // init flames
    for (int i = 0; i < gameState.level.getHeight(); ++i) {
        std::vector<int> row;
        for (int j = 0; j < gameState.level.getWidth(); ++j) {
            row.push_back(0);
        }
        gameState.flames.push_back(row);
    }

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
                if (mcts.isInitialized() == false) {
                    mcts.init(gameState);
                }
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
                bool played = gameState.players[0].play(MOVE_UP, gameState);
                gameState.AIturn = true;
                if (played) {
                    mcts.nextSimulation(MOVE_UP);
                } else {
                    mcts.nextSimulation(NO_ACTION);
                }
            } else if (event.key.code == sf::Keyboard::Down) {
                bool played = gameState.players[0].play(MOVE_DOWN, gameState);
                gameState.AIturn = true;
                if (played) {
                    mcts.nextSimulation(MOVE_DOWN);
                } else {
                    mcts.nextSimulation(NO_ACTION);
                }
            } else if (event.key.code == sf::Keyboard::Left) {
                bool played = gameState.players[0].play(MOVE_LEFT, gameState);
                gameState.AIturn = true;
                if (played) {
                    mcts.nextSimulation(MOVE_LEFT);
                } else {
                    mcts.nextSimulation(NO_ACTION);
                }
            } else if (event.key.code == sf::Keyboard::Right) {
                bool played = gameState.players[0].play(MOVE_RIGHT, gameState);
                gameState.AIturn = true;
                if (played) {
                    mcts.nextSimulation(MOVE_RIGHT);
                } else {
                    mcts.nextSimulation(NO_ACTION);
                }
            } else if (event.key.code == sf::Keyboard::Space) {
                bool played = gameState.players[0].play(PLACE_BOMB, gameState);
                gameState.AIturn = true;
                if (played) {
                    mcts.nextSimulation(PLACE_BOMB);
                } else {
                    mcts.nextSimulation(NO_ACTION);
                }
            }
        }
    }
}

void Game::update()
{
    // Update the players
    int playerSize = gameState.players.size();
    for (int i = 0; i < playerSize; ++i) {
        if (gameState.players[i].getType() == AI) {
            auto start = std::chrono::high_resolution_clock::now();

            std::cout << "Finding best action..." << std::endl;
            Action action = mcts.findBestAction();
            std::cout << "Action: " << action << std::endl;
            gameState.players[i].play(action, gameState);
            mcts.nextSimulation(action);

            auto end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> elapsed = end - start;
            std::cout << "Time taken: " << elapsed.count() << "s" << std::endl;
        }
    }

    // Update the game state
    gameState.update();
}

void Game::render()
{
    // Draw the current game state
    gameState.level.draw(window, zoom);
    
    // Draw the players
    int playerSize = gameState.players.size();
    for (int i = 0; i < playerSize; ++i) {
        gameState.players[i].draw(window, zoom);
    }

    // Draw the bombs
    int bombSize = gameState.bombs.size();
    for (long unsigned i = 0; i < bombSize; ++i) {
        gameState.bombs[i].draw(window, zoom);
    }

    // Draw the flames
    for (long unsigned i = 0; i < gameState.flames.size(); ++i) {
        for (long unsigned j = 0; j < gameState.flames[i].size(); ++j) {
            if (gameState.flames[i][j] == 1) {
                Flame::draw(j, i, window, zoom);
            }
        }
    }

    // Draw the HUD
    HUD::draw(window, *this);
}