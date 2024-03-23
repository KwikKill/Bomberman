#ifndef PATHFINDING_H
#define PATHFINDING_H

#include <SFML/Graphics.hpp>
#include <queue>
#include <vector>
#include <optional>

#include "GameState.h"
#include "Player.h"

class PathFinding
{
private:
    struct PathFindingNode
    {
        int x;
        int y;
        int distance;
        Action action;
        PathFindingNode *parent;
    };

    static std::vector<Action> getLegalMoves(int x, int y, GameState &state, Player player);

public:
    static std::vector<Action> findNearestSafePath(int x, int y, GameState &state, Player player);
    static bool isSafe(int x, int y, GameState &state, Player player);
};

#endif // PATHFINDING_H