#ifndef FLAME_H
#define FLAME_H

#include <SFML/Graphics.hpp>

#define DEFAULT_FLAME_TIMER 1

class Flame
{
public:
    static void draw(int x, int y, sf::RenderWindow &window, int zoom);
};

#endif // FLAME_H