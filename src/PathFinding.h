/* #ifndef PATHFINDING_H
#define PATHFINDING_H

#include <SFML/Graphics.hpp>
#include <queue>
#include <vector>
#include <optional>

#include "Game.h"
#include "Player.h"

class PathFinding
{
private:
    struct Node
    {
        int x;
        int y;
        int distance;
        Node *parent;
    };

    static std::vector<std::pair<int, int>> getLegalMoves(int x, int y, Game &game, std::optional<Player> player = std::nullopt);

public:
    static std::vector<std::pair<int, int>> findNearestSafePath(int x, int y, Game &game, std::optional<Player> player = std::nullopt);
    static bool isSafe(int x, int y, Game &game, std::optional<Player> player = std::nullopt);
};

#endif // PATHFINDING_H */