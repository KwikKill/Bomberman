// Pseudocode
#include <SFML/Graphics.hpp>
#include <iostream>
#include "Game.h"

int main(char argc, char** argv)
{
    srand(time(NULL));
    // Print a message to the console
    std::cout << "Game starting" << std::endl;
    Game game;
    // Level number is the first argument
    int level_nb = 1;
    if (argc > 1)
        level_nb = atoi(argv[1]);
    game.load(level_nb);
    std::cout << "Game loaded" << std::endl;
    game.run();
    std::cout << "Game finished" << std::endl;
    exit(0);
}

