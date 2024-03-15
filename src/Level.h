#include <SFML/Graphics.hpp>

#ifndef LEVEL_H
#define LEVEL_H

#include <string>

#define MAX_SIZE 100

class Level {
private:
    std::string levelData[MAX_SIZE];
    size_t width;
    size_t height;

    std::vector<std::pair<int, int>> spawnPositions;

public:
    void load(int levelNumber);
    void draw(sf::RenderWindow &window);

    bool isEmpty(int x, int y);
    bool isDestroyable(int x, int y);
    bool isundestroyWall(int x, int y);
    void destroyWall(int x, int y);

    size_t getWidth() const { return width; }
    size_t getHeight() const { return height; }
    std::vector<std::pair<int, int>> getSpawnPositions() const {
        return spawnPositions;
    }
};

#endif // LEVEL_H