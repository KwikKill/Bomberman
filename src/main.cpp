// Pseudocode
#include <SFML/Graphics.hpp>
#include <iostream>
#include "Game.h"

int main()
{
    srand(time(NULL));
    // Print a message to the console
    std::cout << "Game starting" << std::endl;
    Game game;
    game.load();
    std::cout << "Game loaded" << std::endl;
    game.run();
    std::cout << "Game finished" << std::endl;
}