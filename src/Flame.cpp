#include "Flame.h"
#include "Game.h"

Flame::Flame(int x, int y, int zoom) {
    this->x = x;
    this->y = y;
    time_left = DEFAULT_FLAME_TIMER;

    if (!texture.loadFromFile("assets/img/flame.png")) {
        // handle error
    }
    sprite.scale(1.0/zoom, 1.0/zoom);
    sprite.setTexture(texture);
}

void Flame::draw(sf::RenderWindow &window, int zoom) {
    sprite.setTexture(texture);
    sprite.setPosition(
        (x * TILE_SIZE)/zoom,
        (y * TILE_SIZE)/zoom
    );
    window.draw(sprite);
}

void Flame::update() {
    time_left--;
}
