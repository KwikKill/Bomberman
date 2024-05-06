# Bomberman Game - AI Project

## Introduction

This project is a simple implementation of the Bomberman game for the AI for Games course at INSA Rennes.
The game is implemented in C++ and uses the SFML library for the graphical interface.
This is only a simple version of the game, with a single player and a single AI enemy.
The player and the AI can move in four directions and place bombs to destroy walls and kill the enemy.

This implementation is a turn-based game, The player moves first, then the AI moves and then the bombs explode.

## Installation

To compile the game, you need to have the SFML library installed on your computer.
You need to have the `PATH_TO_FFML` environment variable set to the SFML library on your computer.
A copy of the library is provided in the `SFML-2.6.1` folder.

To compile the game, you can use the provided `Makefile` by running the following command in the terminal:

```bash
make
```

This will create an executable file called `bin/main`.

## Usage

To run the game, you can use the following command:

```bash
./bin/main {optional map ID}
```

The game will start and you can use the arrow keys to move the player, the space key to place a bomb and the AI will move automatically.
You can change the map by providing a map ID as an argument to the program.
The available maps are stored in the `assets/levels` folder.

## Authors

This project was developed by:
- [TheBloodMan49](https://github.com/TheBloodMan49)
- [KwikKill](https://github.com/KwikKill)

## License

This project is licensed under the CC BY-NC-SA 4.0 License - see the [LICENSE](LICENSE) file for details or visit [Creative Commons](https://creativecommons.org/licenses/by-nc-sa/4.0/).