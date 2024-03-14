#include <string>
#include <fstream>
#include "Level.h"
#include <iostream>
#include <SFML/Graphics.hpp>

void Level::load(int levelNumber) {

    std::string filePath = "assets/levels/" + std::to_string(levelNumber) + ".txt";
    std::ifstream file(filePath);

    if (file.is_open()) {
        std::string line;
        int row = 0;
        width = 0;
        height = 0;

        while (std::getline(file, line) && row < MAX_SIZE) {
            if (line.size() > width) {
                width = line.size();
            }
            levelData[row] = line;
            row++;
        }
        height = row;

        file.close();
    }

    // Find a random "S" tile to spawn the player
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            if (levelData[i][j] == 'S') {
                spawnPositions.push_back(std::make_pair(j, i));
            }
        }
    }
}

void Level::draw(sf::RenderWindow &window) {
    sf::Texture wall1texture;
    if (!wall1texture.loadFromFile("assets/img/wall1.png")) {
        // handle error
    }
    sf::Sprite wall1sprite;
    wall1sprite.setTexture(wall1texture);

    sf::Texture wall2texture;
    if (!wall2texture.loadFromFile("assets/img/wall2.png")) {
        // handle error
    }
    sf::Sprite wall2sprite;
    wall2sprite.setTexture(wall2texture);

    sf::Texture Voidtexture;
    if (!Voidtexture.loadFromFile("assets/img/void.png")) {
        // handle error
    }
    sf::Sprite voidsprite;
    voidsprite.setTexture(Voidtexture);

    for (int i = 0; i < MAX_SIZE; ++i) {
        for (int j = 0; j < levelData[i].size(); ++j) {
            if (levelData[i][j] == 'X') {
                wall1sprite.setPosition(
                    j * wall1texture.getSize().x,
                    i * wall1texture.getSize().y
                );
                window.draw(wall1sprite);
            } else if (levelData[i][j] == '.') {
                wall2sprite.setPosition(
                    j * wall2texture.getSize().x,
                    i * wall2texture.getSize().y
                );
                window.draw(wall2sprite);
            } else {
                voidsprite.setPosition(
                    j * Voidtexture.getSize().x,
                    i * Voidtexture.getSize().y
                );
                window.draw(voidsprite);
            }
        }
    }
}

bool Level::isEmpty(int x, int y) {
    // out of bounds
    if (x < 0 || y < 0 || x >= width || y >= height) {
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
    if (x < 0 || y < 0 || x >= width || y >= height) {
        return false;
    }
    // Breakable wall
    if(levelData[y][x] == '.') {
        return true;
    }
    return false;
}