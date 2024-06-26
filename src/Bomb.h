#ifndef BOMB_H
#define BOMB_H

#include <SFML/Graphics.hpp>
#include "Level.h"
#include "Player.h"
#include <optional>

#define DEFAULT_BOMB_TIMER 3

class Bomb
{
private:
    int x;
    int y;
    int time_left;
    int strength;
    sf::Texture texture;
    sf::Sprite sprite;

    std::optional<int> owner;

    static std::vector<std::string> *bombTextures;

public:
    Bomb();
    Bomb(int x, int y, int timer, int strength, std::optional<int> owner);
    void draw(sf::RenderWindow &window, int zoom);
    // return a vector of all the positions that require a flame
    std::vector<std::pair<int, int>> explode(GameState &state);
    void update();
    int getX() const { return x; }
    int getY() const { return y; }
    int getTimeLeft() const { return time_left; }
    int getStrength() const { return strength; }
    std::optional<int> getOwner() const { return owner; }

    void changeTexture();
};

#endif // BOMB_H