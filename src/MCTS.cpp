#include "GameState.h"
#include "Node.h"
#include "MCTS.h"
#include "Bomb.h"
#include "fast_log.hpp"
#include <cmath>
#include <iostream>
#include <utility>
#include "PathFinding.h"

fast_log fast_lg;

Node* bestChild(Node* node) {
    // Return the child node with the highest value

    double bestValue = -1;

    Node* bestChild = nullptr;

    for (Node* child : node->children) {
        const float log_of_N = fast_lg.log(node->visits);
        double value = (double)child->wins / child->visits + sqrt(2 * log_of_N / child->visits);

        if (value > bestValue) {
            bestValue = value;
            bestChild = child;
        }
    }

    return bestChild;
}


void backpropagate(Node* node, int result) {
    // Update the node's statistics based on the result of the game

    node->visits++;
    node->wins += result;

    if (node->parent != nullptr) {
        backpropagate(node->parent, result);
    }

}

bool isTerminal(const GameState& state, int depth = 0) {
    // Check if the game is over
    if(state.winner != NO_WINNER) {
        return true;
    }
    // Check if the game is more than a certain number of turns
    if(state.turns - depth >= MAX_TURNS) {
        return true;
    }
    return false;
}

std::vector<Action> getPossibleActions(GameState& state) {
    std::vector<Action> possibleActions;

    int playerId = 0;

    if (state.AIturn)
        playerId = 1;

    // Check if the player can move up
    if (state.level.isEmpty(state.players[playerId].getX(), state.players[playerId].getY() - 1)) {
        bool canPlay = true;

        // check if there is a bomb at the position
        for (long unsigned i = 0; i < state.bombs.size(); ++i) {
            if (state.bombs[i].getX() == state.players[playerId].getX() && state.bombs[i].getY() == state.players[playerId].getY() - 1) {
                canPlay = false;
                break;
            }
        }
        // check if there is a player at the position
        for (const auto & player : state.players) {
            if (player.getX() == state.players[playerId].getX() && player.getY() == state.players[playerId].getY() - 1) {
                canPlay = false;
                break;
            }
        }
        if (canPlay) {
            possibleActions.push_back(MOVE_UP);
        }
    }
    // Check if the player can move down
    if (state.level.isEmpty(state.players[playerId].getX(), state.players[playerId].getY() + 1)) {
        bool canPlay = true;
        
        // check if there is a bomb at the position
        for (long unsigned i = 0; i < state.bombs.size(); ++i) {
            if (state.bombs[i].getX() == state.players[playerId].getX() && state.bombs[i].getY() == state.players[playerId].getY() + 1) {
                canPlay = false;
                break;
            }
        }
        // check if there is a player at the position
        for (const auto & player : state.players) {
            if (player.getX() == state.players[playerId].getX() && player.getY() == state.players[playerId].getY() + 1) {
                canPlay = false;
                break;
            }
        }
        if (canPlay) {
            possibleActions.push_back(MOVE_DOWN);
        }
    }
    // Check if the player can move left
    if (state.level.isEmpty(state.players[playerId].getX() - 1, state.players[playerId].getY())) {
        bool canPlay = true;
        
        // check if there is a bomb at the position
        for (long unsigned i = 0; i < state.bombs.size(); ++i) {
            if (state.bombs[i].getX() == state.players[playerId].getX() - 1 && state.bombs[i].getY() == state.players[playerId].getY()) {
                canPlay = false;
                break;
            }
        }
        // check if there is a player at the position
        for (const auto & player : state.players) {
            if (player.getX() == state.players[playerId].getX() - 1 && player.getY() == state.players[playerId].getY()) {
                canPlay = false;
                break;
            }
        }
        if (canPlay) {
            possibleActions.push_back(MOVE_LEFT);
        }
    }
    // Check if the player can move right
    if (state.level.isEmpty(state.players[playerId].getX() + 1, state.players[playerId].getY())) {
        bool canPlay = true;
        
        // check if there is a bomb at the position
        for (long unsigned i = 0; i < state.bombs.size(); ++i) {
            if (state.bombs[i].getX() == state.players[playerId].getX() + 1 && state.bombs[i].getY() == state.players[playerId].getY()) {
                canPlay = false;
                break;
            }
        }
        // check if there is a player at the position
        for (const auto & player : state.players) {
            if (player.getX() == state.players[playerId].getX() + 1 && player.getY() == state.players[playerId].getY()) {
                canPlay = false;
                break;
            }
        }
        if (canPlay) {
            possibleActions.push_back(MOVE_RIGHT);
        }
    }

    // Add other possible actions here
    if (state.players[playerId].getNumBombs() > 0)
        possibleActions.push_back(PLACE_BOMB);
    possibleActions.push_back(NO_ACTION);

    return possibleActions;
}

bool isFullyExpanded(Node* node) {
    // Check if all possible actions have been tried
    return node->children.size() == getPossibleActions(node->state).size();
}

GameState getNewState(const GameState& state, Action action, bool AIturn) {
    // Get the new game state resulting from taking the action
    GameState newState = state;

    if (AIturn) {
        newState.players[1].play(action, newState);
    } else {
        newState.players[0].play(action, newState);
        newState.AIturn = true;
    }

    return newState;
}

int defaultPolicy(GameState state, int depth = 0) {
    // Simulate a random game and return the result


    while (!isTerminal(state)) {
        std::vector<Action> possibleActions = getPossibleActions(state);

        Action action = possibleActions[rand() % possibleActions.size()];

        state = getNewState(state, action);
        
    while (!isTerminal(state, depth)) {
        /*if(!PathFinding::isSafe(state.players[0].getX(), state.players[0].getY(), state, state.players[0])) {
            std::vector<Action> path = PathFinding::findNearestSafePath(state.players[0].getX(), state.players[0].getY(), state, state.players[0]);
            Action actionPlayer = path[0];
            
            state = getNewState(state, actionPlayer, state.AIturn);
            actions.push_back(std::make_pair(state.AIturn, actionPlayer));
        } else {*/
        std::vector<Action> possibleActionsPlayer = getPossibleActions(state);
        Action actionPlayer = possibleActionsPlayer[rand() % possibleActionsPlayer.size()];
        state = getNewState(state, actionPlayer, state.AIturn);
        //}

        std::vector<Action> possibleActionsAI = getPossibleActions(state);
        Action actionAI = possibleActionsAI[rand() % possibleActionsAI.size()];
        state = getNewState(state, actionAI, state.AIturn);

        state.update();
    }

    if (state.winner == NO_WINNER) {
        return 0;
    } else if (state.winner == PLAYER2) {
        return 1;
    } else {
        return -1;
    }
}

Node* expand(Node* node) {
    // Get a list of possible actions from the current state
    std::vector<Action> possibleActions = getPossibleActions(node->state);

    // Remove actions that have already been tried
    for (Node* child : node->children) {
        possibleActions.erase(
            std::remove(
                possibleActions.begin(),
                possibleActions.end(),
                child->actionTaken
            ),
            possibleActions.end()
        );
    }

    // Choose a random action from the remaining actions
    Action action = possibleActions[rand() % possibleActions.size()];

    GameState newState;
    Node* newNode;
    // Get the new game state resulting from taking the action
    if(node->state.AIturn) {
        newState = getNewState(node->state, action, node->state.AIturn);
        newState.update();
        // Create a new node for the new state
        newNode = new Node(newState, action, node, true);
    } else {
        newState = getNewState(node->state, action, node->state.AIturn);
        newNode = new Node(newState, action, node, false);
    }

    // Add the new node to the children of the current node
    node->children.push_back(newNode);

    return newNode;
}

Node* treePolicy(Node* node) {
    while (!isTerminal(node->state, node->state.turns)) {
        if (!isFullyExpanded(node)) {
            return expand(node);
        } else {
            node = bestChild(node);
        }
    }
    return node;
}

Action MCTS::findBestAction(GameState &currentState) {
    Node* root = new Node(currentState, NO_ACTION, nullptr, true);

    for (int i = 0; i < NUM_SIMULATIONS; ++i) {
        Node* node = treePolicy(root);
        int result = defaultPolicy(node->state, node->state.turns);
        backpropagate(node, result);
    }

    if(root->children.size() == 0) {
        std::cout << "No children" << std::endl;
        return NO_ACTION;
    }
    for (Node* child : root->children) {
        std::cout << "Action: " << child->actionTaken << " Wins: " << child->wins << " Visits: " << child->visits << std::endl;
    }

    // Log the action that led to the loss
    /*Node* current = bestChild(root);
    while(current->children.size() > 0) {
        std::cout << "Action of best child : " << current->actionTaken << std::endl;
        current = bestChild(current);
    }*/

    return bestChild(root)->actionTaken;
}