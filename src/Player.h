#ifndef PLAYER_P
#define PLAYER_P

#include <SFML/Graphics.hpp>

#define DEFAULT_PLAYER_SPEED 1
#define DEFAULT_PLAYER_STRENGTH 1
#define DEFAULT_PLAYER_BOMBS 1

// Enum PlayerType
enum PlayerType
{
    PLAYER,
    AI,
};

class GameState;
class Game;

enum Action {
    NO_ACTION,
    MOVE_UP,
    MOVE_DOWN,
    MOVE_LEFT,
    MOVE_RIGHT,
    PLACE_BOMB
};

class Player
{
private:
    int id;
    int x;
    int y;
    int speed;
    sf::Texture texture;
    sf::Sprite sprite;
    PlayerType type;
    bool alive;

    int strength;
    int numBombs;

    void move(int dx, int dy);

public:
    Player(int id, int x, int y, int speed, std::string texturePath, PlayerType type, int zoom);
    void draw(sf::RenderWindow &window, int zoom);
    void play(Action action, GameState &state);

    void update(GameState &gamestate);

    void die();
    bool dropBomb();
    int getX() const { return x; }
    int getY() const { return y; }

    int getStrength() const { return strength; }
    int getNumBombs() const { return numBombs; }
    void addBomb() { numBombs++; }

    void addBonus(int type);

    bool isAlive() const { return alive; }
    bool isLegalMove(int x, int y, GameState &state);
};

#endif // PLAYER_P