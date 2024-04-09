#include "Flame.h"
#include "Game.h"

void Flame::draw(int x, int y, sf::RenderWindow &window, int zoom) {
    sf::Texture texture;
    if (!texture.loadFromFile("assets/img/flame.png")) {
        // handle error
    }
    sf::Sprite sprite;

    sprite.setScale(1.0/zoom, 1.0/zoom);
    sprite.setTexture(texture);
    sprite.setPosition(
        (x * TILE_SIZE)/zoom,
        (y * TILE_SIZE)/zoom
    );
    window.draw(sprite);
}