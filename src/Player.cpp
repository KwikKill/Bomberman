#include <SFML/Graphics.hpp>
#include "Player.h"
#include "Game.h"

Player::Player() {
    x = 0;
    y = 0;
    speed = 0;
    type = PLAYER;
    alive = true;
    strength = DEFAULT_PLAYER_STRENGTH;
    numBombs = DEFAULT_PLAYER_BOMBS;
}

Player::Player(int x, int y, int speed, std::string texturePath, PlayerType type) {
    this->x = x;
    this->y = y;
    this->speed = speed;
    this->type = type;
    alive = true;
    strength = DEFAULT_PLAYER_STRENGTH;
    numBombs = DEFAULT_PLAYER_BOMBS;

    if (!texture.loadFromFile(texturePath)) {
        // handle error
    }
    sprite.setTexture(texture);
}

void Player::draw(sf::RenderWindow &window) {
    sprite.setTexture(texture);
    sprite.setPosition(
        x * TILE_SIZE,
        y * TILE_SIZE
    );
    window.draw(sprite);
}

void Player::move(int dx, int dy) {
    x += dx * speed;
    y += dy * speed;
}

void Player::update(Game &game) {
    if (type == AI) {
        // AI logic
    }
}

bool Player::dropBomb() {
    if (numBombs > 0) {
        numBombs--;
        return true;
    }
    return false;
}

void Player::die() {
    // handle player death
    std::cout << "Player died!" << std::endl;
    alive = false;
}

void Player::addBonus(int type) {
    if (type == STRENGTH) {
        strength++;
    } else if (type == NUMBOMB) {
        numBombs++;
    }
}