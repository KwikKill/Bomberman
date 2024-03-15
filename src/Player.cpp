#include <SFML/Graphics.hpp>
#include "Player.h"
#include "Game.h"
#include "PathFinding.h"

Player::Player() {
    x = 0;
    y = 0;
    speed = 0;
    type = PLAYER;
    alive = true;
    strength = DEFAULT_PLAYER_STRENGTH;
    numBombs = DEFAULT_PLAYER_BOMBS;
}

Player::Player(int x, int y, int speed, std::string texturePath, PlayerType type, int zoom) {
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
    sprite.setScale(1.0/zoom, 1.0/zoom);
}

void Player::draw(sf::RenderWindow &window, int zoom) {
    sprite.setTexture(texture);
    sprite.setPosition(
        (x * TILE_SIZE)/zoom,
        (y * TILE_SIZE)/zoom
    );
    window.draw(sprite);
}

void Player::move(int dx, int dy) {
    x += dx * speed;
    y += dy * speed;
}

void Player::update(Game &game) {
    if (type == AI) {
        if(!PathFinding::isSafe(x, y, game, *this)) {
            std::vector<std::pair<int, int>> path = PathFinding::findNearestSafePath(x, y, game, *this);
            
            if (path.size() > 0) {
                std::pair<int, int> nextMove = path[0];
                
                // move uses the difference between the current position and the next position
                move(nextMove.first-x, nextMove.second-y);
            }
        }
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