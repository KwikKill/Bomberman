#include "GameState.h"
#include <iostream>

void GameState::PlayerCheckBonus(Player &player) {
    for (long unsigned i = 0; i < bonuses.size(); ++i) {
        if (player.getX() == bonuses[i].getX() && player.getY() == bonuses[i].getY()) {
            player.addBonus(bonuses[i].getType());
            bonuses.erase(bonuses.begin() + i);
        }
    }
}

void GameState::update()
{
    //std::cout << "Turn " << turns << std::endl;

    // Update the players
    for (int i = 0; i < players.size(); ++i) {
        PlayerCheckBonus(players[i]);
    }
    
    // Update the bombs
    for (int i = bombs.size() - 1; i >= 0; --i) {
        //std::cout << "bomb " << i << " time left: " << bombs[i].getTimeLeft() << std::endl;
        if(bombs[i].getTimeLeft() > 0) {
            bombs[i].update();
            bombs[i].changeTexture();
        } else {
            // Explode the bomb
            std::vector<std::pair<int, int>> flamePositions = bombs[i].explode(*this);

            // Add the flames to the list of flames
            for (long unsigned j = 0; j < flamePositions.size(); ++j) {
                flames.push_back(
                    Flame(flamePositions[j].first, flamePositions[j].second)
                );
            }
            // add a bomb to the player
            std::optional<int> owner = bombs[i].getOwner();
            if (owner.has_value()) {
                players[owner.value()].addBomb();
            }
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
                    std::vector<std::pair<int, int>> flamePositions = bombs[i].explode(*this);
                    for (long unsigned j = 0; j < flamePositions.size(); ++j) {
                        flames.push_back(
                            Flame(flamePositions[j].first, flamePositions[j].second)
                        );
                    }
                    std::optional<int> owner = bombs[i].getOwner();
                    if (owner.has_value()) {
                        players[owner.value()].addBomb();
                    }
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
    for (int i = 0; i < players.size(); ++i) {
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

    turns++;
    AIturn = false;
}