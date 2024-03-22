#ifndef FLAME_H
#define FLAME_H

#include <SFML/Graphics.hpp>

#define DEFAULT_FLAME_TIMER 1

class Flame
{
private:
    int x;
    int y;
    int time_left;
    
    sf::Texture texture;
    sf::Sprite sprite;

public:
    Flame();
    Flame(int x, int y);
    void draw(sf::RenderWindow &window, int zoom);
    void update();
    int getX() const { return x; }
    int getY() const { return y; }
    int getTimeLeft() const { return time_left; }
    void changeTexture();
};

#endif // FLAME_H