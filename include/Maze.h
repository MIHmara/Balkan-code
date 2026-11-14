#ifndef MAZE_H
#define MAZE_H

#include <random>

// ---------------------------------------------------------------------------
// Maze module (Backend): creating, storing, generating and analysing the maze.
// None of these functions use standard input/output.
// ---------------------------------------------------------------------------

// Value stored in a distance grid for tiles that cannot be reached.
const int UNREACHABLE = -1;

// Every tile of the maze is exactly one of these elements.
enum class Tile
{
    Wall, // solid wall - the player cannot pass through it
    Path, // open corridor
    Door, // special door - opens with a key and shortens the way to the exit
    Key,  // key lying on the floor - picked up automatically
    Exit  // the exit in the bottom-right corner
};

// A cell coordinate: row 0 is the top, column 0 is the left side.
struct Position
{
    int row;
    int col;
};

// The maze is stored in a dynamically allocated two-dimensional array.
// Ownership: the Maze that receives the result of createMaze() must be
// released exactly once with destroyMaze().
struct Maze
{
    int size = 0;           // number of rows (= number of columns)
    Tile** grid = nullptr;  // grid[row][col]
};

// --- Memory management -----------------------------------------------------
Maze createMaze(int size);
void destroyMaze(Maze& maze);

// --- Basic queries ---------------------------------------------------------
Position getStartPosition();
Position getExitPosition(const Maze& maze);
bool isInside(const Maze& maze, Position pos);
bool isSamePosition(Position first, Position second);
bool isWalkable(Tile tile, bool allowDoors);
Tile getTile(const Maze& maze, Position pos);
void setTile(Maze& maze, Position pos, Tile tile);
int countTiles(const Maze& maze, Tile tile);

// --- Generation ------------------------------------------------------------
void fillWithWalls(Maze& maze);
void carvePassages(Maze& maze, std::mt19937& rng);
void openBorderNooks(Maze& maze, std::mt19937& rng);
void openExit(Maze& maze);
int placeDoors(Maze& maze, int maxDoors, int minSaving);
int placeKeys(Maze& maze, std::mt19937& rng);
void generateMaze(Maze& maze, unsigned int seed);

// --- Path finding (Breadth-First Search) -----------------------------------
int** createDistanceGrid(int size);
void destroyDistanceGrid(int** distances, int size);
void computeDistances(const Maze& maze, Position source, bool allowDoors, int** distances);
int shortestPathLength(const Maze& maze, bool allowDoors);

#endif // MAZE_H
