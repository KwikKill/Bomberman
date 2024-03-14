#ifndef BOMB_H
#define BOMB_H

#include <SFML/Graphics.hpp>
#include "Level.h"
#include "Player.h"

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

    Player *owner;

public:
    Bomb();
    Bomb(int x, int y, int timer, int strength, std::string texturePath);
    void draw(sf::RenderWindow &window);
    void explode(Level &level);
    int getX() const { return x; }
    int getY() const { return y; }
    int getTimeLeft() const { return time_left; }
};

#endif // BOMB_H