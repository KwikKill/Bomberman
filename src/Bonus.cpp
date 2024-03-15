#include "Bonus.h"
#include "Game.h"

Bonus::Bonus() {
    x = 0;
    y = 0;
    type = STRENGTH;
}

Bonus::Bonus(int x, int y, BonusType type) {
    this->x = x;
    this->y = y;
    this->type = type;

    if (!texture.loadFromFile("assets/img/bonus/" + std::to_string(type) + ".png")) {
        // handle error
    }
    sprite.setTexture(texture);
}

void Bonus::draw(sf::RenderWindow &window) {
    sprite.setTexture(texture);
    sprite.setPosition(
        x * TILE_SIZE,
        y * TILE_SIZE
    );
    window.draw(sprite);
}

void Bonus::update() {
    // Do nothing
}

BonusType Bonus::getRandomType() {
    return static_cast<BonusType>(rand() % 2);
}