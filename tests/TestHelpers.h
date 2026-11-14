#ifndef TEST_HELPERS_H
#define TEST_HELPERS_H

#include "Game.h"

// Builds a maze from text rows, so tests can use small hand-made mazes.
// '#' wall, '.' path, 'D' door, 'K' key, 'E' exit.
Maze buildMazeFromText(const char* const rows[], int size);

// Plays the shortest route (doors closed) from the player to the exit and
// returns the number of moves made.
int walkShortestRoute(GameState& game);

#endif // TEST_HELPERS_H
