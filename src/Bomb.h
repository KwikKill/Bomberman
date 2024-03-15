#ifndef BOMB_H
#define BOMB_H

#include <SFML/Graphics.hpp>
#include "Level.h"
#include "Player.h"
#include "Bonus.h"

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
    Bomb(int x, int y, int timer, int strength, std::string texturePath, Player *owner);
    void draw(sf::RenderWindow &window);
    void explode(Level &level, Player *players, int numPlayers, std::vector<Bonus> &bonuses);
    void update();
    int getX() const { return x; }
    int getY() const { return y; }
    int getTimeLeft() const { return time_left; }
    int getStrength() const { return strength; }
    Player *getOwner() const { return owner; }
};

#endif // BOMB_H