#ifndef LEVEL_P
#define LEVEL_P

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

class Player
{
private:
    int x;
    int y;
    int speed;
    sf::Texture texture;
    sf::Sprite sprite;
    PlayerType type;
    bool alive;

    int strength;
    int numBombs;

public:
    Player();
    Player(int x, int y, int speed, std::string texturePath, PlayerType type);
    void draw(sf::RenderWindow &window);
    void move(int dx, int dy);
    void update();
    void die();
    bool dropBomb();
    int getX() const { return x; }
    int getY() const { return y; }

    int getStrength() const { return strength; }
    int getNumBombs() const { return numBombs; }
    void addBomb() { numBombs++; }

    void addBonus(int type);

    bool isAlive() const { return alive; }
};

#endif // LEVEL_P