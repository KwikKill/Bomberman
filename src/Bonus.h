#ifndef BONUS_H
#define BONUS_H

#include <SFML/Graphics.hpp>
#include "Level.h"
#include "Player.h"

enum BonusType
{
    STRENGTH,
    NUMBOMB,
};

class Bonus
{
private:
    int x;
    int y;
    sf::Texture texture;
    sf::Sprite sprite;
    BonusType type;

public:
    Bonus();
    Bonus(int x, int y, BonusType type, int zoom);
    void draw(sf::RenderWindow &window, int zoom);
    void update();
    int getX() const { return x; }
    int getY() const { return y; }
    BonusType getType() const { return type; }

    static BonusType getRandomType();

};

#endif // BONUS_H