#include "GameState.h"
#include "Node.h"
#include "MCTS.h"
#include "Bomb.h"
#include "fast_log.hpp"

#include <cmath>
#include <iostream>
#include <utility>
#include <thread>
#include <mutex>
#include <fstream>

fast_log fast_lg;

Node* bestChild(Node* node) {
    // Return the child node with the highest value based on the UCB1 formula
    double bestValue = -1000000000;
    Node* bestChild = nullptr;

    for (Node* child : node->children) {
        const float log_of_N = fast_lg.log(node->visits);
        double value;
        if (node->state.AIturn) {
            value = (double)child->wins / child->visits + sqrt(2 * log_of_N / child->visits);
        } else {
            value = (1.0 - (double)child->wins / child->visits) + sqrt(2 * log_of_N / child->visits);
        }

        if (value > bestValue) {
            bestValue = value;
            bestChild = child;
        }
    }

    if (bestChild == nullptr) {
        std::cout << "No best child, This might be an error" << std::endl;
        // exit the program
        exit(1);
    } else {
        //std::cout << "Best child: " << bestChild->actionTaken << " Wins: " << bestChild->wins << " Visits: " << bestChild->visits << std::endl;
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

bool canMoveInDirection(GameState& state, int playerId, int dx, int dy) {
    // Check if the player can move in the given direction
    int new_x = state.players[playerId].getX() + dx;
    int new_y = state.players[playerId].getY() + dy;
    if (state.level.isEmpty(new_x, new_y)) {
        // check if there is a bomb at the position
        int bombSize = state.bombs.size();
        for (long unsigned i = 0; i < bombSize; ++i) {
            if (state.bombs[i].getX() == new_x && state.bombs[i].getY() == new_y) {
                return false;
            }
        }
        // check if there is a player at the position
        for (const auto & player : state.players) {
            if (player.getX() == new_x && player.getY() == new_y) {
                return false;
            }
        }
        return true;
    }
    return false;

}

std::vector<Action> getPossibleActions(GameState& state) {
    std::vector<Action> possibleActions;
    // Reserve space for the maximum number of possible actions
    possibleActions.reserve(6);

    int playerId = state.AIturn ? 1 : 0;

    // Check if the player can move up
    if (canMoveInDirection(state, playerId, 0, -1)) {
        possibleActions.push_back(MOVE_UP);
    }
    // Check if the player can move down
    if (canMoveInDirection(state, playerId, 0, 1)) {
        possibleActions.push_back(MOVE_DOWN);
    }
    // Check if the player can move left
    if (canMoveInDirection(state, playerId, -1, 0)) {
        possibleActions.push_back(MOVE_LEFT);
    }
    // Check if the player can move right
    if (canMoveInDirection(state, playerId, 1, 0)) {
        possibleActions.push_back(MOVE_RIGHT);
    }

    // Add other possible actions here
    if (state.players[playerId].getNumBombs() > 0)
        possibleActions.push_back(PLACE_BOMB);
    possibleActions.push_back(NO_ACTION);

    return possibleActions;
}

bool isFullyExpanded(Node* node) {
    // Check if all possible actions have been tried
    if(node->isFullyExpanded) {
        return true;
    } else if (node->children.size() == getPossibleActions(node->state).size()) {
        node->isFullyExpanded = true;
        return true;
    }
    return false;
}

void getNewState(GameState& state, Action action, bool AIturn) {
    // Get the new game state resulting from taking the action

    if (AIturn) {
        state.players[1].play(action, state);
    } else {
        state.players[0].play(action, state);
        state.AIturn = true;
    }
}

int defaultPolicy(GameState state, int depth = 0) {
    // Simulate a random game and return the result
    while (!isTerminal(state, depth)) {
        if(state.AIturn) {
            std::vector<Action> possibleActionsAI = getPossibleActions(state);

            Action actionTakenAI = possibleActionsAI[rand() % possibleActionsAI.size()];

            getNewState(state, actionTakenAI, state.AIturn);
            // Update the board after the AI has played
            state.update();
        } else {
            std::vector<Action> possibleActionsPlayer = getPossibleActions(state);

            Action actionPlayer = possibleActionsPlayer[rand() % possibleActionsPlayer.size()];
            
            getNewState(state, actionPlayer, state.AIturn);
        }
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

    // Remove actions that have already a node in the tree
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

    bool AIturn = node->state.AIturn;

    GameState newState = node->state;

    getNewState(newState, action, node->state.AIturn);
    // If the AI is playing, update the board
    if (AIturn) {
        newState.update();
    }
    Node* newNode = new Node(action, newState, node);    

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

void log_tree(Node* node, int depth = 0, std::ostream& output = std::cout) {
    for (int i = 0; i < depth; ++i) {
        output << "  ";
    }
    output << "Action: " << node->actionTaken << " By " << !node->state.AIturn << " Wins: " << node->wins << " Visits: " << node->visits << std::endl;
    for (Node* child : node->children) {
        log_tree(child, depth + 1, output);
    }
}

double calculateWinPercentage(Node* node) {
    if (node->visits == 0) {
        return 0.0;  // to avoid division by zero
    }
    return static_cast<double>(node->wins + node->visits) / static_cast<double>(2*node->visits);
}

Action MCTS::findBestAction() {
    // print the current state
    std::cout << "Current state: " << root->state.turns << std::endl;
    std::cout << "AIturn: " << root->state.AIturn << std::endl;

    for (int i = 0; i < NUM_SIMULATIONS; ++i) {
        Node* node = treePolicy(root);
        int result = defaultPolicy(node->state, node->state.turns);
        backpropagate(node, result);
    }

    if(root->children.size() == 0) {
        std::cout << "No children" << std::endl;
        return NO_ACTION;
    }

    log_tree(root, 0);
    
    Node* bestChildNode = bestChild(root);
    // Write what the AI thinks is the it's winning percentage
    std::cout << "Winning percentage: " << calculateWinPercentage(bestChildNode)*100 << std::endl;

    return bestChildNode->actionTaken;
}

void MCTS::init(GameState& currentState) {
    root = new Node(currentState, NO_ACTION, nullptr);
}

void MCTS::nextSimulation(Action action) {
    if (root == nullptr) {
        return;
    }
    // Find the child node with the given action
    for (Node* child : root->children) {
        if (child->actionTaken == action) {
            root = child;
            // Remove this node from his parent children
            root->parent->children.erase(
                std::remove(
                    root->parent->children.begin(),
                    root->parent->children.end(),
                    root
                ),
                root->parent->children.end()
            );
            // Delete the current node and its children
            delete root->parent;
            root->parent = nullptr;
            return;
        }
    }
    

    // If the child node is not found
    std::cout << "This action seems to be umpossible, please take a closer look" << std::endl;
    std::cout << "Action: " << action << std::endl;
    log_tree(root);
    exit(1);
}