#include "Bomb.h"
#include "Game.h"

Bomb::Bomb() {
    x = 0;
    y = 0;
    time_left = DEFAULT_BOMB_TIMER;
    strength = 1;
}

Bomb::Bomb(int x, int y, int timer, int strength, std::string texturePath) {
    this->x = x;
    this->y = y;
    time_left = timer;
    this->strength = strength;

    if (!texture.loadFromFile(texturePath)) {
        // handle error
    }
    sprite.setTexture(texture);
}

void Bomb::draw(sf::RenderWindow &window) {
    sprite.setPosition(
        x * TILE_SIZE,
        y * TILE_SIZE
    );
    window.draw(sprite);
}