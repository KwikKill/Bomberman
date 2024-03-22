#include "GameState.h"

void GameState::PlayerCheckBonus(Player &player) {
    for (long unsigned i = 0; i < bonuses.size(); ++i) {
        if (player.getX() == bonuses[i].getX() && player.getY() == bonuses[i].getY()) {
            player.addBonus(bonuses[i].getType());
            bonuses.erase(bonuses.begin() + i);
        }
    }
}