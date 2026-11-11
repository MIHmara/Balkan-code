#ifndef GAME_H
#define GAME_H

#include "Maze.h"

// ---------------------------------------------------------------------------
// Game module (Backend): game state, rules and calculations.
// None of these functions use standard input/output.
// ---------------------------------------------------------------------------

enum class Direction
{
    Up,
    Down,
    Left,
    Right
};

// Outcome of a single move attempt.
enum class MoveResult
{
    Moved,        // the player moved to an open tile
    HitWall,      // blocked by a wall - the player stays in place
    HitEdge,      // blocked by the border of the maze
    DoorLocked,   // a door is ahead but the player has no key
    DoorOpened,   // a key was used to open a door and the player went through
    KeyCollected, // the player stepped on a key and picked it up
    ReachedExit,  // the player reached the exit - the game is won
    GameOver      // the game is already finished, no more moves are accepted
};

struct Player
{
    Position pos = { 0, 0 };
    int keys = 0;            // keys currently in the inventory
    int keysCollected = 0;   // keys picked up during the whole game
    int doorsOpened = 0;
    int moves = 0;           // successful moves only
    int blockedAttempts = 0; // attempts to walk into walls, edges or locked doors
};

struct GameState
{
    Maze maze;
    Player player;
    unsigned int seed = 0;
    int parMoves = 0;      // shortest path to the exit without using doors
    bool finished = false; // true once the exit is reached
};

void initGame(GameState& game, Maze maze, unsigned int seed);
GameState startNewGame(int size, unsigned int seed);
void endGame(GameState& game);

Position getNextPosition(Position from, Direction direction);
bool canMove(const GameState& game, Direction direction);
MoveResult movePlayer(GameState& game, Direction direction);
bool isAtExit(const GameState& game);
int distanceToExit(const GameState& game);
int getStarRating(const GameState& game);

#endif // GAME_H