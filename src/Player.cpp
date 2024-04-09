#include <SFML/Graphics.hpp>
#include "Player.h"
#include "Game.h"
//#include "PathFinding.h"
#include "GameState.h"
#include "Node.h"
#include "MCTS.h"

Player::Player(int id, int x, int y, int speed, std::string texturePath, PlayerType type, int zoom) {
    this->id = id;
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

bool Player::play(Action action, GameState &state) {
        switch (action) {
        case MOVE_UP:
            if(isLegalMove(x, y-1, state)) {
                move(0, -1);
                return true;
            }
            return false;
        case MOVE_DOWN:
            if(isLegalMove(x, y+1, state)) {
                move(0, 1);
                return true;
            }
            return false;
        case MOVE_LEFT:
            if(isLegalMove(x-1, y, state)) {
                move(-1, 0);
                return true;
            }
            return false;
        case MOVE_RIGHT:
            if(isLegalMove(x+1, y, state)) {
                move(1, 0);
                return true;
            }
            return false;
        case PLACE_BOMB:
            if (dropBomb()) {
                state.bombs.push_back(
                    Bomb(
                        x,
                        y,
                        DEFAULT_BOMB_TIMER,
                        strength,
                        id
                    )
                );
                return true;
            }
            return false;
        default:
            return false;
    }
    return true;

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
    //std::cout << "Player died!" << std::endl;
    alive = false;
}

void Player::addBonus(int type) {
    if (type == STRENGTH) {
        strength++;
    } else if (type == NUMBOMB) {
        numBombs++;
    }
}

bool Player::isLegalMove(int x, int y, GameState &state) {
    if(state.level.isEmpty(x, y)) {
        // Check if there is a bomb at the position
        for (long unsigned i = 0; i < state.bombs.size(); ++i) {
            if (state.bombs[i].getX() == x && state.bombs[i].getY() == y) {
                return false;
            }
        }
        // Check if there is a player at the position
        for (int i = 0; i < 2; ++i) {
            if(this->x == x && this->y == y) {
                return false;
            }
            if (state.players[i].getX() == x && state.players[i].getY() == y) {
                return false;
            }
        }
        return true;
    }
    return false;
}