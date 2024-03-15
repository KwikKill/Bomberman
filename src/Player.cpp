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
        if(!isSafe(x, y, game)) {
            std::vector<std::pair<int, int>> legalMoves = getLegalMoves(game);
            if (legalMoves.size() > 0) {
                int index = rand() % legalMoves.size();
                move(legalMoves[index].first, legalMoves[index].second);
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

bool Player::isSafe(int x, int y, Game &game) {
    // if is a legal move
    if(!game.isLegalMove(x, y, *this)) {
        return false;
    }
    for (Bomb &bomb : game.getBombs()) {
        // Check the bomb explosion area
        for (int i = 0; i < 4; ++i) {
            for (int j = 1; j <= bomb.getStrength(); ++j) {
                int dx = 0;
                int dy = 0;
                if (i == 0) {
                    dx = j;
                } else if (i == 1) {
                    dx = -j;
                } else if (i == 2) {
                    dy = j;
                } else if (i == 3) {
                    dy = -j;
                }

                int new_x = x + dx;
                int new_y = y + dy;

                if (game.getLevel().isDestroyable(new_x, new_y)) {
                    break;
                } else if (game.getLevel().isundestroyWall(new_x, new_y)) {
                    break;
                } else if (new_x == bomb.getX() && new_y == bomb.getY()) {
                    return false;
                }
            }
        }
    }
    return true;
}

std::vector<std::pair<int, int>> Player::getLegalMoves(Game &game) {
    std::vector<std::pair<int, int>> legalMoves;
    // Check the 4 possible moves
    if (game.isLegalMove(x + 1, y, *this)) {
        legalMoves.push_back(std::make_pair(1, 0));
    }
    if (game.isLegalMove(x - 1, y, *this)) {
        legalMoves.push_back(std::make_pair(-1, 0));
    }
    if (game.isLegalMove(x, y + 1, *this)) {
        legalMoves.push_back(std::make_pair(0, 1));
    }
    if (game.isLegalMove(x, y - 1, *this)) {
        legalMoves.push_back(std::make_pair(0, -1));
    }
    return legalMoves;
}