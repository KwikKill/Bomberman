#include <SFML/Graphics.hpp>
#include <iostream>
#include "Level.h"
#include "Game.h"
#include "Player.h"
#include "Bomb.h"
#include "Bonus.h"

Game::Game() {
    hasMoved = false;
    winner = NO_WINNER;
    level = Level();
}

void Game::load(unsigned int level_nb) {
    // Create a new level
    level.load(level_nb);

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
            // print player strength and number of bombs
            std::cout << "Strength: " << players[0].getStrength() << " Bombs: " << players[0].getNumBombs() << std::endl;
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
                        &players[0]
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
        players[i].update();
    }

    // Update the bombs
    for (int i = 0; i < bombs.size(); ++i) {
        std::cout << bombs[i].getTimeLeft() << std::endl;
        if(bombs[i].getTimeLeft() > 0) {
            bombs[i].update();
            bombs[i].changeTexture();
        } else {
            // Explode the bomb
            std::vector<std::pair<int, int>> flamePositions = bombs[i].explode(level, players, 2, bonuses);
            
            std::cout << "Flame positions: " << flamePositions.size() << std::endl;
            
            // Add the flames to the list of flames
            for (int j = 0; j < flamePositions.size(); ++j) {
                flames.push_back(
                    Flame(flamePositions[j].first, flamePositions[j].second)
                );
            }
            // add a bomb to the player
            bombs[i].getOwner()->addBomb();
            // Remove the bomb
            bombs.erase(bombs.begin() + i);
        }
    }

    // Update the flames
    for (int i = flames.size() -1; i >= 0; --i) {
        std::cout << "flame ttl " << flames[i].getTimeLeft() << std::endl;
        if (flames[i].getTimeLeft() > 0) {
            flames[i].update();
        } else {
            std::cout << "Flame removed " << i << std::endl;
            flames.erase(flames.begin() + i);
        }
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

    // Draw the bonuses
    for (int i = 0; i < bonuses.size(); ++i) {
        bonuses[i].draw(window);
    }

    // Draw the bombs
    for (int i = 0; i < bombs.size(); ++i) {
        bombs[i].draw(window);
    }

    // Draw the flames
    for (int i = 0; i < flames.size(); ++i) {
        flames[i].draw(window);
    }
}

bool Game::isLegalMove(int x, int y) {
    if(level.isEmpty(x, y)) {
        // Check if there is a bomb at the position
        for (int i = 0; i < bombs.size(); ++i) {
            if (bombs[i].getX() == x && bombs[i].getY() == y) {
                return false;
            }
        }
        return true;
    }
    return false;
}

void Game::PlayerCheckBonus(Player &player) {
    for (int i = 0; i < bonuses.size(); ++i) {
        if (player.getX() == bonuses[i].getX() && player.getY() == bonuses[i].getY()) {
            player.addBonus(bonuses[i].getType());
            bonuses.erase(bonuses.begin() + i);
        }
    }
}