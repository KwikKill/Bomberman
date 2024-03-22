/* #include "PathFinding.h"
#include <optional>

std::vector<std::pair<int, int>> PathFinding::getLegalMoves(int x, int y, Game &game, std::optional<Player> player) {
    std::vector<std::pair<int, int>> legalMoves;
    // Check the 4 possible moves
    if (game.isLegalMove(x + 1, y, player)) {
        legalMoves.push_back(std::make_pair(1, 0));
    }
    if (game.isLegalMove(x - 1, y, player)) {
        legalMoves.push_back(std::make_pair(-1, 0));
    }
    if (game.isLegalMove(x, y + 1, player)) {
        legalMoves.push_back(std::make_pair(0, 1));
    }
    if (game.isLegalMove(x, y - 1, player)) {
        legalMoves.push_back(std::make_pair(0, -1));
    }
    return legalMoves;
}

bool PathFinding::isSafe(int x, int y, Game &game, std::optional<Player> player) {
    // if is a legal move
    if(!game.isLegalMove(x, y, player)) {
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

std::vector<std::pair<int, int>> PathFinding::findNearestSafePath(int x, int y, Game &game, std::optional<Player> player) {
    std::vector<std::pair<int, int>> path;
    std::queue<Node*> queue;
    queue.push(
        new Node{
            x,
            y,
            0,
            nullptr
        }
    );
    std::vector<std::vector<bool>> visited(game.getLevel().getWidth(), std::vector<bool>(game.getLevel().getHeight(), false));
    visited[x][y] = true;

    while (!queue.empty()) {
        Node* current = queue.front();
        queue.pop();

        if (isSafe(current->x, current->y, game, player)) {
            while (current->parent != nullptr) {
                path.push_back(std::make_pair(current->x, current->y));
                current = current->parent;
            }
            std::reverse(path.begin(), path.end());
            return path;
        }

        std::vector<std::pair<int, int>> legalMoves = getLegalMoves(current->x, current->y, game, player);
        for (std::pair<int, int> move : legalMoves) {
            int new_x = current->x + move.first;
            int new_y = current->y + move.second;
            if (!visited[new_x][new_y]) {
                visited[new_x][new_y] = true;
                Node *newNode = new Node{new_x, new_y, current->distance + 1, current};
                queue.push(newNode);
            }
        }
    }
    return path;
} */