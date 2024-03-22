#include "GameState.h"
#include "Node.h"
#include "MCTS.h"
#include "Bomb.h"
#include <cmath>
#include <iostream>

Node* bestChild(Node* node) {
    // Return the child node with the highest value

    double bestValue = -1;

    Node* bestChild = nullptr;

    for (Node* child : node->children) {
        double value = (double)child->wins / child->visits + sqrt(2 * log(node->visits) / child->visits);

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

    return;
}

bool isTerminal(GameState state) {
    // Check if the game is over
    if(state.winner != NO_WINNER) {
        return true;
    }
    // Check if the game is more than a certain number of turns
    if(state.turns >= MAX_TURNS) {
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
        for (int i = 0; i < 2; ++i) {
            if (state.players[i].getX() == state.players[playerId].getX() && state.players[i].getY() == state.players[playerId].getY() - 1) {
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
        for (int i = 0; i < 2; ++i) {
            if (state.players[i].getX() == state.players[playerId].getX() && state.players[i].getY() == state.players[playerId].getY() + 1) {
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
        for (int i = 0; i < 2; ++i) {
            if (state.players[i].getX() == state.players[playerId].getX() - 1 && state.players[i].getY() == state.players[playerId].getY()) {
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
        for (int i = 0; i < 2; ++i) {
            if (state.players[i].getX() == state.players[playerId].getX() + 1 && state.players[i].getY() == state.players[playerId].getY()) {
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

GameState getNewState(const GameState& state, Action action) {
    // Get the new game state resulting from taking the action
    GameState newState = state;

    newState.players[1].play(action, newState);

    return newState;
}

int defaultPolicy(GameState state) {
    // Simulate a random game and return the result

    while (!isTerminal(state)) {
        std::vector<Action> possibleActions = getPossibleActions(state);

        Action action = possibleActions[rand() % possibleActions.size()];

        state = getNewState(state, action);
    }

    if (state.winner == NO_WINNER) {
        return 0;
    } else if (state.winner == PLAYER1) {
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
        possibleActions.erase(std::remove(possibleActions.begin(), possibleActions.end(), child->actionTaken), possibleActions.end());
    }

    // Choose a random action from the remaining actions
    Action action = possibleActions[rand() % possibleActions.size()];

    // Get the new game state resulting from taking the action
    GameState newState = getNewState(node->state, action);

    // Create a new node for the new state
    Node* newNode = new Node(newState, action, node);

    // Add the new node to the children of the current node
    node->children.push_back(newNode);

    return newNode;
}

Node* treePolicy(Node* node) {
    while (!isTerminal(node->state)) {
        if (!isFullyExpanded(node)) {
            return expand(node);
        } else {
            node = bestChild(node);
        }
    }
    return node;
}

Action MCTS::findBestAction(GameState currentState) {
    Node* root = new Node(currentState, NO_ACTION, nullptr);

    for (int i = 0; i < NUM_SIMULATIONS; ++i) {
        std::cout << "Simulation " << i << std::endl;
        Node* node = treePolicy(root);
        std::cout << "Node: " << node->state.players[1].getX() << " " << node->state.players[1].getY() << std::endl;
        int result = defaultPolicy(node->state);
        std::cout << "Result: " << result << std::endl;
        backpropagate(node, result);
        std::cout << "Backpropagated" << std::endl;
    }

    return bestChild(root)->actionTaken;
}