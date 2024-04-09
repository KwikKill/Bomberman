# Level creation

## Base

The base of the level is a 2D array of string. Each string is a character that represents a tile. There can't be more than 100 lines and 100 characters per line.

## Tiles

The tiles are the following:

- `X`: Unbreakable wall, the player can't go through it nor destroy it. The id of this tile is `wall1`.
- `.`: Breakable wall, the player can destroy it by shooting it. The id of this tile is `wall2`.
- `S`: The starting position of a player. This tile has no id as it's not a real tile and is replaced by a void.
- Every other character is considered as a void. The id of this tile is `void`.
- `B`: This represents a bomb ready to explode. This is not a real tile and is replaced by a void at the level loading. This is for testing purposes and should not be used in a level.