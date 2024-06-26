#include <string>
#include <fstream>
#include "Level.h"
#include "Game.h"
#include <iostream>
#include <SFML/Graphics.hpp>

std::vector<Bomb> Level::load(unsigned int levelNumber) {

    std::string filePath = "assets/levels/" + std::to_string(levelNumber) + ".txt";
    std::ifstream file(filePath);

    if (file.is_open()) {
        std::string line;
        int row = 0;
        width = 0;
        height = 0;

        while (std::getline(file, line)) {
            if (line.size() > width) {
                width = line.size();
            }
            levelData.push_back(line);
            row++;
        }
        height = row;

        file.close();
    }

    // If this is a bomb tile "B", add a bomb to the game and replace the tile with a space
    std::vector<Bomb> bombs;
    for (size_t i = 0; i < height; ++i) {
        for (size_t j = 0; j < width; ++j) {
            if (levelData[i][j] == 'B') {
                bombs.push_back(
                    Bomb(
                        j,
                        i,
                        DEFAULT_BOMB_TIMER,
                        1,
                        std::nullopt
                    )
                );
                levelData[i][j] = ' ';
            }
        }
    }

    // Find a random "S" tile to spawn the player
    for (size_t i = 0; i < height; ++i) {
        for (size_t j = 0; j < width; ++j) {
            if (levelData[i][j] == 'S') {
                spawnPositions.push_back(std::make_pair(j, i));
            }
        }
    }

    return bombs;
}

void Level::draw(sf::RenderWindow &window, int zoom) {
    sf::Texture wall1texture;
    if (!wall1texture.loadFromFile("assets/img/wall1.png")) {
        // handle error
    }
    sf::Sprite wall1sprite;
    wall1sprite.scale(1.0/zoom, 1.0/zoom);
    wall1sprite.setTexture(wall1texture);

    sf::Texture wall2texture;
    if (!wall2texture.loadFromFile("assets/img/wall2.png")) {
        // handle error
    }
    sf::Sprite wall2sprite;
    wall2sprite.scale(1.0/zoom, 1.0/zoom);
    wall2sprite.setTexture(wall2texture);

    sf::Texture Voidtexture;
    if (!Voidtexture.loadFromFile("assets/img/void.png")) {
        // handle error
    }
    sf::Sprite voidsprite;
    voidsprite.scale(1.0/zoom, 1.0/zoom);
    voidsprite.setTexture(Voidtexture);
    
    sf::Texture bonus0texture;
    if (!bonus0texture.loadFromFile("assets/img/bonus/0.png")) {
        // handle error
    }
    sf::Sprite bonus0sprite;
    bonus0sprite.scale(1.0/zoom, 1.0/zoom);
    bonus0sprite.setTexture(bonus0texture);

    sf::Texture bonus1texture;
    if (!bonus1texture.loadFromFile("assets/img/bonus/1.png")) {
        // handle error
    }
    sf::Sprite bonus1sprite;
    bonus1sprite.scale(1.0/zoom, 1.0/zoom);
    bonus1sprite.setTexture(bonus1texture);

    for (long unsigned i = 0; i < levelData.size(); ++i) {
        for (long unsigned j = 0; j < levelData[i].size(); ++j) {
            if (levelData[i][j] == 'X') {
                wall1sprite.setPosition(
                    j * wall1texture.getSize().x/zoom,
                    i * wall1texture.getSize().y/zoom
                );
                window.draw(wall1sprite);
            } else if (levelData[i][j] == '.') {
                wall2sprite.setPosition(
                    j * wall2texture.getSize().x/zoom,
                    i * wall2texture.getSize().y/zoom
                );
                window.draw(wall2sprite);
            } else if (levelData[i][j] == '0') {
                voidsprite.setPosition(
                    j * Voidtexture.getSize().x/zoom,
                    i * Voidtexture.getSize().y/zoom
                );
                window.draw(voidsprite);
                bonus0sprite.setPosition(
                    j * bonus0texture.getSize().x/zoom,
                    i * bonus0texture.getSize().y/zoom
                );
                window.draw(bonus0sprite);
            } else if (levelData[i][j] == '1') {
                voidsprite.setPosition(
                    j * Voidtexture.getSize().x/zoom,
                    i * Voidtexture.getSize().y/zoom
                );
                window.draw(voidsprite);
                bonus1sprite.setPosition(
                    j * bonus1texture.getSize().x/zoom,
                    i * bonus1texture.getSize().y/zoom
                );
                window.draw(bonus1sprite);
            } else {
                voidsprite.setPosition(
                    j * Voidtexture.getSize().x/zoom,
                    i * Voidtexture.getSize().y/zoom
                );
                window.draw(voidsprite);
            }
        }
    }
}

bool Level::isEmpty(int x, int y) {
    // out of bounds
    if (x < 0 || y < 0 || x >= (int) width || y >= (int) height) {
        return false;
    }
    // Umbreakable wall
    if(levelData[y][x] == 'X') {
        return false;
    }
    // Breakable wall
    if(levelData[y][x] == '.') {
        return false;
    }
    return true;
}

bool Level::isDestroyable(int x, int y) {
    // out of bounds
    if (x < 0 || y < 0 || x >= (int) width || y >= (int) height) {
        return false;
    }
    // Breakable wall
    if(levelData[y][x] == '.') {
        return true;
    }
    return false;
}

bool Level::isundestroyWall(int x, int y) {
    return levelData[y][x] == 'X';
}

void Level::destroyWall(int x, int y) {
    levelData[y][x] = ' ';
}

void Level::addBonus(int x, int y, BonusType type) {
    levelData[y][x] = std::to_string(type)[0];
}

int Level::isBonus(int x, int y) {
    if(levelData[y][x] == '0') {
        return 0;
    }
    if(levelData[y][x] == '1') {
        return 1;
    }
    return -1;
}

void Level::removeBonus(int x, int y) {
    levelData[y][x] = ' ';
}

BonusType Level::getRandomType() {
    return static_cast<BonusType>(rand() % 2);
}