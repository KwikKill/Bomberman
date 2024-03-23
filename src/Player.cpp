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

void Player::play(Action action, GameState &state) {
        switch (action) {
        case MOVE_UP:
            if(isLegalMove(x, y-1, state)) {
                move(0, -1);
            }
            break;
        case MOVE_DOWN:
            if(isLegalMove(x, y+1, state)) {
                move(0, 1);
            }
            break;
        case MOVE_LEFT:
            if(isLegalMove(x-1, y, state)) {
                move(-1, 0);
            }
            break;
        case MOVE_RIGHT:
            if(isLegalMove(x+1, y, state)) {
                move(1, 0);
            }
            break;
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
            }
            break;
        default:
            break;
    }

}

void Player::update(GameState &gamestate) {
    if (type == AI) {
        /*if(!PathFinding::isSafe(x, y, game, *this)) {
            std::vector<std::pair<int, int>> path = PathFinding::findNearestSafePath(x, y, game, *this);
            
            if (path.size() > 0) {
                std::pair<int, int> nextMove = path[0];
                
                // move uses the difference between the current position and the next position
                move(nextMove.first-x, nextMove.second-y);
            }
        }*/
        MCTS mtcs = MCTS();
        std::cout << "Finding best action..." << std::endl;
        Action action = mtcs.findBestAction(gamestate);
        std::cout << "Action: " << action << std::endl;
        play(action, gamestate);
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