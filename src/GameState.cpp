#include "GameState.h"
#include <iostream>

void GameState::PlayerCheckBonus(Player &player) {
    int bonus = level.isBonus(player.getX(), player.getY());
    if (bonus != -1) {
        player.addBonus(bonus);
        level.removeBonus(player.getX(), player.getY());
    }
}

void GameState::update()
{
    //std::cout << "Turn " << turns << std::endl;

    // Update the players
    for (int i = 0; i < 2; ++i) {
        PlayerCheckBonus(players[i]);
    }

    //std::cout << "AAAAA " << std::endl;

    // Reset the flames
    for (long unsigned i = 0; i < flames.size(); ++i) {
        for (long unsigned j = 0; j < flames[i].size(); ++j) {
            flames[i][j] = 0;
        }
    }

    //std::cout << "BBBBB " << std::endl;
    
    // Update the bombs
    int bombSize = bombs.size();
    for (int i = bombSize - 1; i >= 0; --i) {
        //std::cout << "bomb " << i << " time left: " << bombs[i].getTimeLeft() << std::endl;
        if(bombs[i].getTimeLeft() > 0) {
            bombs[i].update();
            bombs[i].changeTexture();
        } else {
            // Explode the bomb
            std::vector<std::pair<int, int>> flamePositions = bombs[i].explode(*this);

            // Add the flames to the list of flames
            int flamePosistionSize = flamePositions.size();
            for (long unsigned j = 0; j < flamePosistionSize; ++j) {
                flames[flamePositions[j].second][flamePositions[j].first] = 1;
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

    //std::cout << "CCCCC " << std::endl;

    // Recursively explode the bombs
    bool explosion = true;
    while (explosion) {
        explosion = false;
        int bombSize = bombs.size();
        for (int i = bombSize - 1; i >= 0; --i) {
            if (flames[bombs[i].getY()][bombs[i].getX()] == 1) {
                std::vector<std::pair<int, int>> flamePositions = bombs[i].explode(*this);
                int flamePosistionSize = flamePositions.size();
                for (long unsigned j = 0; j < flamePosistionSize; ++j) {
                    flames[flamePositions[j].second][flamePositions[j].first] = 1;
                }
                std::optional<int> owner = bombs[i].getOwner();
                if (owner.has_value()) {
                    players[owner.value()].addBomb();
                }
                bombs.erase(bombs.begin() + i);
                explosion = true;
            }
        }
    }

    //std::cout << "DDDDD " << std::endl;

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

    turns++;
    AIturn = false;
}