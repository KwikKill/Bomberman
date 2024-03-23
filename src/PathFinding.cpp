#include "PathFinding.h"
#include <optional>
#include <iostream>

std::vector<Action> PathFinding::getLegalMoves(int x, int y, GameState &state, Player player) {
    std::vector<Action> legalMoves;
    // Check the 4 possible moves
    if (player.isLegalMove(x + 1, y, state)) {
        legalMoves.push_back(MOVE_RIGHT);
    }
    if (player.isLegalMove(x - 1, y, state)) {
        legalMoves.push_back(MOVE_LEFT);
    }
    if (player.isLegalMove(x, y + 1, state)) {
        legalMoves.push_back(MOVE_DOWN);
    }
    if (player.isLegalMove(x, y - 1, state)) {
        legalMoves.push_back(MOVE_UP);
    }
    return legalMoves;
}

bool PathFinding::isSafe(int x, int y, GameState &state, Player player) {
    // if is a legal move
    if(!player.isLegalMove(x, y, state)) {
        return false;
    }
    for (Bomb &bomb : state.bombs) {
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

                if (state.level.isDestroyable(new_x, new_y)) {
                    break;
                } else if (state.level.isundestroyWall(new_x, new_y)) {
                    break;
                } else if (new_x == bomb.getX() && new_y == bomb.getY()) {
                    return false;
                }
            }
        }
    }
    return true;
}

std::vector<Action> PathFinding::findNearestSafePath(int x, int y, GameState &state, Player player) {
    std::vector<Action> path;
    std::queue<PathFindingNode*> queue;
    queue.push(
        new PathFindingNode{
            x,
            y,
            0,
            NO_ACTION,
            nullptr
        }
    );
    std::vector<std::vector<bool>> visited(state.level.getWidth(), std::vector<bool>(state.level.getHeight(), false));
    visited[x][y] = true;

    while (!queue.empty()) {
        PathFindingNode* current = queue.front();
        queue.pop();

        if (isSafe(current->x, current->y, state, player)) {
            while (current->parent != nullptr) {
                path.push_back(current->action);
                current = current->parent;
            }
            std::reverse(path.begin(), path.end());
            return path;
        }

        std::vector<Action> legalMoves = getLegalMoves(current->x, current->y, state, player);
        for (Action action : legalMoves) {
            int dx = 0;
            int dy = 0;
            if (action == MOVE_UP) {
                dy = -1;
            } else if (action == MOVE_DOWN) {
                dy = 1;
            } else if (action == MOVE_LEFT) {
                dx = -1;
            } else if (action == MOVE_RIGHT) {
                dx = 1;
            }

            int new_x = current->x + dx;
            int new_y = current->y + dy;

            if (!visited[new_x][new_y]) {
                visited[new_x][new_y] = true;
                queue.push(
                    new PathFindingNode{
                        new_x,
                        new_y,
                        current->distance + 1,
                        action,
                        current
                    }
                );
            }
        }
    }
    path.push_back(NO_ACTION);
    return path;
}