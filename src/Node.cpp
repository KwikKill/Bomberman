#include "GameState.h"
#include "Node.h"

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
}

bool isFullyExpanded(Node* node) {
    // Check if all possible actions have been tried
}

std::vector<Action> getPossibleActions(const GameState& state) {
    // Get possible actions
    
}

GameState getNewState(const GameState& state, Action action) {
    // Get the new game state resulting from taking the action
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