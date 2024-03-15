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
    hasMoved = false;
    winner = NO_WINNER;
    level = Level();
}

void Game::load(unsigned int level_nb) {
    // Create a new level
    level.load(level_nb);

    zoom = 1.0;

    int X_size = level.getWidth() * TILE_SIZE;
    int Y_size = level.getHeight() * TILE_SIZE;
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
    std::vector<std::pair<int, int>> spawnPositions = level.getSpawnPositions();
    int randomIndex = rand() % spawnPositions.size();

    // Create the players and set their positions
    players[0] = Player(
        spawnPositions[randomIndex].first,
        spawnPositions[randomIndex].second,
        DEFAULT_PLAYER_SPEED,
        "assets/img/player.png",
        PLAYER,
        zoom
    );
    players[1] = Player(
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
        if (winner == NO_WINNER) {
            hasMoved = false;
            processEvents();

            window.clear(sf::Color::Black); // Clear the window with black color

            render();
            if (hasMoved) {
                update();
            }

            window.display(); // End the current frame and display everything
        } else {
            std::cout << "The winner is: ";
            if (winner == PLAYER1) {
                std::cout << "Player 1" << std::endl;
            } else if (winner == PLAYER2) {
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
                if(isLegalMove(players[0].getX(), players[0].getY() - 1)) {
                    players[0].move(0, -1);
                    PlayerCheckBonus(players[0]);
                }
                hasMoved = true;
            } else if (event.key.code == sf::Keyboard::Down) {
                if(isLegalMove(players[0].getX(), players[0].getY() + 1)) {
                    players[0].move(0, 1);
                    PlayerCheckBonus(players[0]);
                }
                hasMoved = true;
            } else if (event.key.code == sf::Keyboard::Left) {
                if(isLegalMove(players[0].getX() - 1, players[0].getY())) {
                    players[0].move(-1, 0);
                    PlayerCheckBonus(players[0]);
                }
                hasMoved = true;
            } else if (event.key.code == sf::Keyboard::Right) {
                if(isLegalMove(players[0].getX() + 1, players[0].getY())) {
                    players[0].move(1, 0);
                    PlayerCheckBonus(players[0]);
                }
                hasMoved = true;
            } else if (event.key.code == sf::Keyboard::Space) {
                if(players[0].dropBomb()) {
                    // Create a new bomb
                    bombs.push_back(Bomb(
                        players[0].getX(),
                        players[0].getY(),
                        DEFAULT_BOMB_TIMER,
                        players[0].getStrength(),
                        &players[0],
                        zoom
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
        players[i].update(*this);
    }

    // Update the bombs
    for (int i = bombs.size() - 1; i >= 0; --i) {
        if(bombs[i].getTimeLeft() > 0) {
            bombs[i].update();
            bombs[i].changeTexture();
        } else {
            // Explode the bomb
            std::vector<std::pair<int, int>> flamePositions = bombs[i].explode(level, players, 2, bonuses, zoom);

            // Add the flames to the list of flames
            for (long unsigned j = 0; j < flamePositions.size(); ++j) {
                flames.push_back(
                    Flame(flamePositions[j].first, flamePositions[j].second, zoom)
                );
            }
            // add a bomb to the player
            bombs[i].getOwner()->addBomb();
            // Remove the bomb
            bombs.erase(bombs.begin() + i);
        }
    }
    
    // Recursively explode the bombs
    bool explosion = true;
    while (explosion) {
        explosion = false;
        for (int i = bombs.size() - 1; i >= 0; --i) {
            // if the bomb is on a flame, explode it
            for (long unsigned j = 0; j < flames.size(); ++j) {
                if (bombs[i].getX() == flames[j].getX() && bombs[i].getY() == flames[j].getY()) {
                    std::vector<std::pair<int, int>> flamePositions = bombs[i].explode(level, players, 2, bonuses, zoom);
                    for (long unsigned j = 0; j < flamePositions.size(); ++j) {
                        flames.push_back(
                            Flame(flamePositions[j].first, flamePositions[j].second, zoom)
                        );
                    }
                    bombs[i].getOwner()->addBomb();
                    bombs.erase(bombs.begin() + i);
                    explosion = true;
                    break;
                }
            }
        }
    }

    // Update the flames
    for (int i = flames.size() -1; i >= 0; --i) {
        if (flames[i].getTimeLeft() > 0) {
            flames[i].update();
        } else {
            flames.erase(flames.begin() + i);
        }
    }

    int numAlive = 0;
    for (int i = 0; i < 2; ++i) {
        if (players[i].isAlive()) {
            numAlive++;
        }
    }
    if (numAlive == 0) {
        winner = DRAW;
    } else if (numAlive == 1) {
        if (players[0].isAlive()) {
            winner = PLAYER1;
        } else {
            winner = PLAYER2;
        }
    }
}

void Game::render()
{
    // Draw the current game state
    level.draw(window, zoom);
    
    // Draw the players
    for (int i = 0; i < 2; ++i) {
        players[i].draw(window, zoom);
    }

    // Draw the bonuses
    for (long unsigned i = 0; i < bonuses.size(); ++i) {
        bonuses[i].draw(window, zoom);
    }

    // Draw the bombs
    for (long unsigned i = 0; i < bombs.size(); ++i) {
        bombs[i].draw(window, zoom);
    }

    // Draw the flames
    for (long unsigned i = 0; i < flames.size(); ++i) {
        flames[i].draw(window, zoom);
    }

    // Draw the HUD
    HUD::draw(window, *this);
}

bool Game::isLegalMove(int x, int y, std::optional<Player> player) {
    if(level.isEmpty(x, y)) {
        // Check if there is a bomb at the position
        for (long unsigned i = 0; i < bombs.size(); ++i) {
            if (bombs[i].getX() == x && bombs[i].getY() == y) {
                return false;
            }
        }
        // Check if there is a player at the position
        for (int i = 0; i < 2; ++i) {
            if(player.has_value() && player.value().getX() == x && player.value().getY() == y) {
                continue;
            }
            if (players[i].getX() == x && players[i].getY() == y) {
                return false;
            }
        }
        return true;
    }
    return false;
}

void Game::PlayerCheckBonus(Player &player) {
    for (long unsigned i = 0; i < bonuses.size(); ++i) {
        if (player.getX() == bonuses[i].getX() && player.getY() == bonuses[i].getY()) {
            player.addBonus(bonuses[i].getType());
            bonuses.erase(bonuses.begin() + i);
        }
    }
}