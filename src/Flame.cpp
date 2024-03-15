#include "Flame.h"
#include "Game.h"

Flame::Flame(int x, int y) {
    this->x = x;
    this->y = y;
    time_left = DEFAULT_FLAME_TIMER;

    if (!texture.loadFromFile("assets/img/flame.png")) {
        // handle error
    }
    sprite.setTexture(texture);
}

void Flame::draw(sf::RenderWindow &window) {
    sprite.setTexture(texture);
    sprite.setPosition(
        x * TILE_SIZE,
        y * TILE_SIZE
    );
    window.draw(sprite);
}

void Flame::update() {
    time_left--;
}
