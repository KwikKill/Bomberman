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
                gameState.turns++;
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
                if(isLegalMove(gameState.players[0].getX(), gameState.players[0].getY() - 1)) {
                    gameState.players[0].move(0, -1);
                }
                gameState.AIturn = true;
            } else if (event.key.code == sf::Keyboard::Down) {
                if(isLegalMove(gameState.players[0].getX(), gameState.players[0].getY() + 1)) {
                    gameState.players[0].move(0, 1);
                }
                gameState.AIturn = true;
            } else if (event.key.code == sf::Keyboard::Left) {
                if(isLegalMove(gameState.players[0].getX() - 1, gameState.players[0].getY())) {
                    gameState.players[0].move(-1, 0);
                }
                gameState.AIturn = true;
            } else if (event.key.code == sf::Keyboard::Right) {
                if(isLegalMove(gameState.players[0].getX() + 1, gameState.players[0].getY())) {
                    gameState.players[0].move(1, 0);
                }
                gameState.AIturn = true;
            } else if (event.key.code == sf::Keyboard::Space) {
                if(gameState.players[0].dropBomb()) {
                    // Create a new bomb
                    gameState.bombs.push_back(Bomb(
                        gameState.players[0].getX(),
                        gameState.players[0].getY(),
                        DEFAULT_BOMB_TIMER,
                        gameState.players[0].getStrength(),
                        &gameState.players[0]
                    ));
                }
            }
        }
    }
}

void Game::update()
{
    // Update the players
    for (int i = 0; i < 2; ++i) {
        //gameState.players[i].update(*this);
        gameState.PlayerCheckBonus(gameState.players[i]);   
    }

    // Update the bombs
    for (int i = gameState.bombs.size() - 1; i >= 0; --i) {
        if(gameState.bombs[i].getTimeLeft() > 0) {
            gameState.bombs[i].update();
            gameState.bombs[i].changeTexture();
        } else {
            // Explode the bomb
            std::vector<std::pair<int, int>> flamePositions = gameState.bombs[i].explode(gameState.level, gameState.players, 2, gameState.bonuses, zoom);

            // Add the flames to the list of flames
            for (long unsigned j = 0; j < flamePositions.size(); ++j) {
                gameState.flames.push_back(
                    Flame(flamePositions[j].first, flamePositions[j].second, zoom)
                );
            }
            // add a bomb to the player
            gameState.bombs[i].getOwner()->addBomb();
            // Remove the bomb
            gameState.bombs.erase(gameState.bombs.begin() + i);
        }
    }
    
    // Recursively explode the bombs
    bool explosion = true;
    while (explosion) {
        explosion = false;
        for (int i = gameState.bombs.size() - 1; i >= 0; --i) {
            // if the bomb is on a flame, explode it
            for (long unsigned j = 0; j < gameState.flames.size(); ++j) {
                if (gameState.bombs[i].getX() == gameState.flames[j].getX() && gameState.bombs[i].getY() == gameState.flames[j].getY()) {
                    std::vector<std::pair<int, int>> flamePositions = gameState.bombs[i].explode(gameState.level, gameState.players, 2, gameState.bonuses, zoom);
                    for (long unsigned j = 0; j < flamePositions.size(); ++j) {
                        gameState.flames.push_back(
                            Flame(flamePositions[j].first, flamePositions[j].second, zoom)
                        );
                    }
                    gameState.bombs[i].getOwner()->addBomb();
                    gameState.bombs.erase(gameState.bombs.begin() + i);
                    explosion = true;
                    break;
                }
            }
        }
    }

    // Update the flames
    for (int i = gameState.flames.size() -1; i >= 0; --i) {
        if (gameState.flames[i].getTimeLeft() > 0) {
            gameState.flames[i].update();
        } else {
            gameState.flames.erase(gameState.flames.begin() + i);
        }
    }

    int numAlive = 0;
    for (int i = 0; i < 2; ++i) {
        if (gameState.players[i].isAlive()) {
            numAlive++;
        }
    }
    if (numAlive == 0) {
        gameState.winner = DRAW;
    } else if (numAlive == 1) {
        if (gameState.players[0].isAlive()) {
            gameState.winner = PLAYER1;
        } else {
            gameState.winner = PLAYER2;
        }
    }
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

bool Game::isLegalMove(int x, int y, std::optional<Player> player) {
    if(gameState.level.isEmpty(x, y)) {
        // Check if there is a bomb at the position
        for (long unsigned i = 0; i < gameState.bombs.size(); ++i) {
            if (gameState.bombs[i].getX() == x && gameState.bombs[i].getY() == y) {
                return false;
            }
        }
        // Check if there is a player at the position
        for (int i = 0; i < 2; ++i) {
            if(player.has_value() && player.value().getX() == x && player.value().getY() == y) {
                continue;
            }
            if (gameState.players[i].getX() == x && gameState.players[i].getY() == y) {
                return false;
            }
        }
        return true;
    }
    return false;
}