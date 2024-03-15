#include "Bomb.h"
#include "Game.h"
#include "Bonus.h"

Bomb::Bomb() {
    x = 0;
    y = 0;
    time_left = DEFAULT_BOMB_TIMER;
    strength = 1;
}

Bomb::Bomb(int x, int y, int timer, int strength, std::string texturePath, Player *owner) {
    this->x = x;
    this->y = y;
    time_left = timer;
    this->strength = strength;
    this->owner = owner;

    if (!texture.loadFromFile(texturePath)) {
        // handle error
    }
    sprite.setTexture(texture);
}

void Bomb::draw(sf::RenderWindow &window) {
    sprite.setTexture(texture);
    sprite.setPosition(
        x * TILE_SIZE,
        y * TILE_SIZE
    );
    window.draw(sprite);
}

void Bomb::update() {
    time_left--;
}

void Bomb::explode(Level &level, Player *players, int numPlayers, Bonus *bonuses, int *numBonuses) {
    // explode in all 4 directions
    for (int i = 0; i < 4; ++i) {
        for (int j = 1; j <= strength; ++j) {
            int dx = 0;
            int dy = 0;
            if (i == 0) {
                dx = j;
            } else if (i == 1) {
                dx = -j;
            } else if (i == 2) {
                dy = j;
            } else if (i == 3) {
                dy = -j;
            }

            int new_x = x + dx;
            int new_y = y + dy;

            if (level.isDestroyable(new_x, new_y)) {
                level.destroyWall(new_x, new_y);
                if (rand() % 100 < 20) {
                    std::cout << "Bonus " << new_x << " " << new_y << std::endl;
                    bonuses[*numBonuses] = Bonus(new_x, new_y, Bonus::getRandomType());
                    (*numBonuses)++;
                    std::cout << "Bonus : " << numBonuses << std::endl;
                }
                break;
            } else {
                for (int k = 0; k < numPlayers; ++k) {
                    if (players[k].getX() == new_x && players[k].getY() == new_y) {
                        players[k].die();
                    }
                }
                if (level.isundestroyWall(new_x, new_y)) {
                    break;
                }
            }
        }
    }

    
}