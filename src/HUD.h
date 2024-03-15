#ifndef HUD_H
#define HUD_H

#include <SFML/Graphics.hpp>

#define HUD_HEIGHT 64

class Game;

class HUD
{
public:
    HUD();
    static void draw(sf::RenderWindow &window, Game &game);
};
#endif // HUD_H