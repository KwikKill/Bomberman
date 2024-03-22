#include "GameState.h"
#include "Node.h"
#include "Bomb.h"

Node* bestChild(Node* node) {
    // Return the child node with the highest value
}

int defaultPolicy(GameState state) {
    // Simulate a random game and return the result
}

void backpropagate(Node* node, int result) {
    // Update the node's statistics based on the result of the game
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
        bool canPlay = false;

        // check if there is a bomb at the position
        for (long unsigned i = 0; i < state.bombs.size(); ++i) {
            if (state.bombs[i].getX() == state.players[playerId].getX() && state.bombs[i].getY() == state.players[playerId].getY() - 1) {
                canPlay = true;
                break;
            }
        }
        // check if there is a player at the position
        for (int i = 0; i < 2; ++i) {
            if (state.players[i].getX() == state.players[playerId].getX() && state.players[i].getY() == state.players[playerId].getY() - 1) {
                canPlay = true;
                break;
            }
        }
        if (canPlay) {
            possibleActions.push_back(MOVE_UP);
        }
    }
    // Check if the player can move down
    if (state.level.isEmpty(state.players[playerId].getX(), state.players[playerId].getY() + 1)) {
        bool canPlay = false;
        
        // check if there is a bomb at the position
        for (long unsigned i = 0; i < state.bombs.size(); ++i) {
            if (state.bombs[i].getX() == state.players[playerId].getX() && state.bombs[i].getY() == state.players[playerId].getY() + 1) {
                canPlay = true;
                break;
            }
        }
        // check if there is a player at the position
        for (int i = 0; i < 2; ++i) {
            if (state.players[i].getX() == state.players[playerId].getX() && state.players[i].getY() == state.players[playerId].getY() + 1) {
                canPlay = true;
                break;
            }
        }
        if (canPlay) {
            possibleActions.push_back(MOVE_DOWN);
        }
    }
    // Check if the player can move left
    if (state.level.isEmpty(state.players[playerId].getX() - 1, state.players[playerId].getY())) {
        bool canPlay = false;
        
        // check if there is a bomb at the position
        for (long unsigned i = 0; i < state.bombs.size(); ++i) {
            if (state.bombs[i].getX() == state.players[playerId].getX() - 1 && state.bombs[i].getY() == state.players[playerId].getY()) {
                canPlay = true;
                break;
            }
        }
        // check if there is a player at the position
        for (int i = 0; i < 2; ++i) {
            if (state.players[i].getX() == state.players[playerId].getX() - 1 && state.players[i].getY() == state.players[playerId].getY()) {
                canPlay = true;
                break;
            }
        }
        if (canPlay) {
            possibleActions.push_back(MOVE_LEFT);
        }
    }
    // Check if the player can move right
    if (state.level.isEmpty(state.players[playerId].getX() + 1, state.players[playerId].getY())) {
        bool canPlay = false;
        
        // check if there is a bomb at the position
        for (long unsigned i = 0; i < state.bombs.size(); ++i) {
            if (state.bombs[i].getX() == state.players[playerId].getX() + 1 && state.bombs[i].getY() == state.players[playerId].getY()) {
                canPlay = true;
                break;
            }
        }
        // check if there is a player at the position
        for (int i = 0; i < 2; ++i) {
            if (state.players[i].getX() == state.players[playerId].getX() + 1 && state.players[i].getY() == state.players[playerId].getY()) {
                canPlay = true;
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

    return possibleActions;
}

bool isFullyExpanded(Node* node) {
    // Check if all possible actions have been tried
    return node->children.size() == getPossibleActions(node->state).size();
}

GameState getNewState(const GameState& state, Action action) {
    // Get the new game state resulting from taking the action
    GameState newState = state;

    switch (action) {
        case MOVE_UP:
            newState.players[1].move(0, -1);
            break;
        case MOVE_DOWN:
            newState.players[1].move(0, 1);
            break;
        case MOVE_LEFT:
            newState.players[1].move(-1, 0);
            break;
        case MOVE_RIGHT:
            newState.players[1].move(1, 0);
            break;
        case PLACE_BOMB:
            newState.bombs.push_back(
                Bomb(
                    newState.players[1].getX(),
                    newState.players[1].getY(),
                    DEFAULT_BOMB_TIMER,
                    newState.players[1].getStrength(),
                    &newState.players[1]
                )
            );
            break;
        default:
            break;
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

Action MCTS(GameState currentState) {
    Node* root = new Node(currentState, NO_ACTION, nullptr);

    for (int i = 0; i < NUM_SIMULATIONS; ++i) {
        Node* node = treePolicy(root);
        int result = defaultPolicy(node->state);
        backpropagate(node, result);
    }

    return bestChild(root)->actionTaken;
}