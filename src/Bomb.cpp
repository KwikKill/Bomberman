#include "Bomb.h"
#include "Game.h"
#include "Bonus.h"

std::vector<std::string> *Bomb::bombTextures = new std::vector<std::string> {
    "assets/img/bomb1.png",
    "assets/img/bomb2.png",
    "assets/img/bomb3.png",
};

Bomb::Bomb() {
    x = 0;
    y = 0;
    time_left = DEFAULT_BOMB_TIMER;
    strength = 1;
}

Bomb::Bomb(int x, int y, int timer, int strength, Player *owner) {
    this->x = x;
    this->y = y;
    time_left = timer;
    this->strength = strength;
    this->owner = owner;

    std::cout << "Time left : " << time_left << std::endl;
    std::cout << "Size : " << bombTextures->size() << std::endl;
    int textureIndex = time_left;
    if (textureIndex > (int) bombTextures->size() - 1) {
        textureIndex = bombTextures->size() - 1;
    }
    std::string texturePath = bombTextures->at(textureIndex);

    if (!texture.loadFromFile(texturePath)) {
        // handle error
    }
    sprite.setTexture(texture);
}

void Bomb::draw(sf::RenderWindow &window, int zoom) {
    sprite.setScale(1.0/zoom, 1.0/zoom);
    sprite.setTexture(texture);
    sprite.setPosition(
        (x * TILE_SIZE)/zoom,
        (y * TILE_SIZE)/zoom
    );
    window.draw(sprite);
}

void Bomb::update() {
    time_left--;
}

std::vector<std::pair<int, int>> Bomb::explode(Level &level, Player *players, int numPlayers, std::vector<Bonus> &bonuses) {
    std::vector<std::pair<int, int>> flamePositions;

    // Add flame position for the bomb itself
    flamePositions.push_back(std::make_pair(x, y));

    // Check if the bomb is in the same position as a player
    for (int i = 0; i < numPlayers; ++i) {
        if (players[i].getX() == x && players[i].getY() == y) {
            players[i].die();
        }
    }

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
                if (rand() % 100 < BONUS_SPAWN_CHANCE) {
                    std::cout << "Bonus " << new_x << " " << new_y << std::endl;
                    bonuses.push_back(
                        Bonus(new_x, new_y, Bonus::getRandomType())
                    );
                    std::cout << "Bonus : " << bonuses.size() << std::endl;
                }
                flamePositions.push_back(std::make_pair(new_x, new_y));
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
                flamePositions.push_back(std::make_pair(new_x, new_y));
            }
        }
    }
    return flamePositions;
}

void Bomb::changeTexture() {
    if (time_left > (int) bombTextures->size() - 1) {
        if (!texture.loadFromFile(bombTextures->at(bombTextures->size() - 1))) {
            // handle error
        }
    } else {
        if (!texture.loadFromFile(bombTextures->at(time_left))) {
            // handle error
        }        
    }
}