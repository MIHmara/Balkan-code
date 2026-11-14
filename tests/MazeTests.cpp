#include "AllTests.h"
#include "GameConfig.h"
#include "Maze.h"
#include "TestFramework.h"
#include "TestHelpers.h"

// Seeds used for the random-maze tests (fixed, so results are repeatable).
static const unsigned int TEST_SEEDS[] = {1, 7, 42, 2026, 585319, 999999};
static const int TEST_SEED_COUNT = 6;

// Hand-made 10x10 maze: the only way to the exit is through the door at (0,2).
// The key at (1,0) opens it. Route with the door: 9 steps right + 9 down = 18.
static const char* const DOOR_MAZE[10] = {
    "..D.......",
    "K########.",
    "#########.",
    "#########.",
    "#########.",
    "#########.",
    "#########.",
    "#########.",
    "#########.",
    "#########E"};

// Counts walkable non-door tiles and how many of them are reachable from start.
static bool areAllOpenTilesReachable(const Maze& maze)
{
    int** distances = createDistanceGrid(maze.size);
    computeDistances(maze, getStartPosition(), false, distances);
    bool allReachable = true;
    for (int row = 0; row < maze.size; ++row)
    {
        for (int col = 0; col < maze.size; ++col)
        {
            if (isWalkable(maze.grid[row][col], false) && distances[row][col] == UNREACHABLE)
            {
                allReachable = false;
            }
        }
    }
    destroyDistanceGrid(distances, maze.size);
    return allReachable;
}

static bool areMazesEqual(const Maze& first, const Maze& second)
{
    if (first.size != second.size)
    {
        return false;
    }
    for (int row = 0; row < first.size; ++row)
    {
        for (int col = 0; col < first.size; ++col)
        {
            if (first.grid[row][col] != second.grid[row][col])
            {
                return false;
            }
        }
    }
    return true;
}

// ---------------------------------------------------------------- TC-M ----

void testCreateAndDestroyMaze()
{
    Maze maze = createMaze(MIN_MAZE_SIZE);
    CHECK(maze.size == MIN_MAZE_SIZE);
    CHECK(maze.grid != nullptr);
    CHECK(countTiles(maze, Tile::Wall) == MIN_MAZE_SIZE * MIN_MAZE_SIZE);

    destroyMaze(maze);
    CHECK(maze.grid == nullptr);
    CHECK(maze.size == 0);
    destroyMaze(maze); // a second call must be safe
    CHECK(maze.grid == nullptr);
}

void testIsInside()
{
    Maze maze = createMaze(10);
    CHECK(isInside(maze, Position{0, 0}));
    CHECK(isInside(maze, Position{9, 9}));
    CHECK(!isInside(maze, Position{-1, 0}));
    CHECK(!isInside(maze, Position{0, -1}));
    CHECK(!isInside(maze, Position{10, 5}));
    CHECK(!isInside(maze, Position{5, 10}));
    destroyMaze(maze);
}

void testIsWalkable()
{
    CHECK(!isWalkable(Tile::Wall, false) && !isWalkable(Tile::Wall, true));
    CHECK(isWalkable(Tile::Path, false));
    CHECK(isWalkable(Tile::Key, false));
    CHECK(isWalkable(Tile::Exit, false));
    CHECK(!isWalkable(Tile::Door, false));
    CHECK(isWalkable(Tile::Door, true));
}

void testStartAndExitForAllSizes()
{
    for (int size = MIN_MAZE_SIZE; size <= MAX_MAZE_SIZE; ++size)
    {
        Maze maze = createMaze(size);
        generateMaze(maze, 42);
        CHECK(getTile(maze, getStartPosition()) == Tile::Path);
        CHECK(getTile(maze, getExitPosition(maze)) == Tile::Exit);
        CHECK(countTiles(maze, Tile::Exit) == 1);
        destroyMaze(maze);
    }
}

void testExitReachableWithoutDoors()
{
    for (int size = MIN_MAZE_SIZE; size <= MAX_MAZE_SIZE; ++size)
    {
        for (int index = 0; index < TEST_SEED_COUNT; ++index)
        {
            Maze maze = createMaze(size);
            generateMaze(maze, TEST_SEEDS[index]);
            CHECK(shortestPathLength(maze, false) > 0);
            destroyMaze(maze);
        }
    }
}

void testAllOpenTilesConnected()
{
    for (int size = MIN_MAZE_SIZE; size <= MAX_MAZE_SIZE; size += 5)
    {
        Maze maze = createMaze(size);
        generateMaze(maze, 2026);
        CHECK(areAllOpenTilesReachable(maze));
        destroyMaze(maze);
    }
}

void testSameSeedSameMaze()
{
    Maze first = createMaze(30);
    Maze second = createMaze(30);
    generateMaze(first, 12345);
    generateMaze(second, 12345);
    CHECK(areMazesEqual(first, second));
    destroyMaze(first);
    destroyMaze(second);
}

void testDifferentSeedsDifferentMazes()
{
    Maze first = createMaze(30);
    Maze second = createMaze(30);
    generateMaze(first, 1);
    generateMaze(second, 2);
    CHECK(!areMazesEqual(first, second));
    destroyMaze(first);
    destroyMaze(second);
}

void testDoorsArePlacedCorrectly()
{
    for (int size = MIN_MAZE_SIZE; size <= MAX_MAZE_SIZE; ++size)
    {
        Maze maze = createMaze(size);
        generateMaze(maze, 7);
        CHECK(countTiles(maze, Tile::Door) <= size / 10);

        // Every door must stand between two open tiles (up/down or left/right).
        for (int row = 0; row < size; ++row)
        {
            for (int col = 0; col < size; ++col)
            {
                if (maze.grid[row][col] != Tile::Door)
                {
                    continue;
                }
                const Position up{row - 1, col};
                const Position down{row + 1, col};
                const Position left{row, col - 1};
                const Position right{row, col + 1};
                const bool vertical = isInside(maze, up) && isInside(maze, down) &&
                                      isWalkable(getTile(maze, up), false) &&
                                      isWalkable(getTile(maze, down), false);
                const bool horizontal = isInside(maze, left) && isInside(maze, right) &&
                                        isWalkable(getTile(maze, left), false) &&
                                        isWalkable(getTile(maze, right), false);
                CHECK(vertical || horizontal);
            }
        }
        destroyMaze(maze);
    }
}

void testDoorsShortenTheRoute()
{
    int mazesWithDoors = 0;
    for (int size = MIN_MAZE_SIZE; size <= MAX_MAZE_SIZE; ++size)
    {
        Maze maze = createMaze(size);
        generateMaze(maze, 2026);
        if (countTiles(maze, Tile::Door) > 0)
        {
            ++mazesWithDoors;
            CHECK(shortestPathLength(maze, true) < shortestPathLength(maze, false));
        }
        destroyMaze(maze);
    }
    CHECK(mazesWithDoors > 0); // the feature is really used
}

void testKeysArePlacedCorrectly()
{
    for (int size = MIN_MAZE_SIZE; size <= MAX_MAZE_SIZE; ++size)
    {
        Maze maze = createMaze(size);
        generateMaze(maze, 999999);
        CHECK(countTiles(maze, Tile::Key) == countTiles(maze, Tile::Door));
        CHECK(getTile(maze, getStartPosition()) != Tile::Key);
        CHECK(getTile(maze, getExitPosition(maze)) != Tile::Key);
        CHECK(areAllOpenTilesReachable(maze)); // keys are reachable without doors
        destroyMaze(maze);
    }
}

void testShortestPathOnHandMadeMaze()
{
    Maze maze = buildMazeFromText(DOOR_MAZE, 10);
    CHECK(shortestPathLength(maze, false) == UNREACHABLE);
    CHECK(shortestPathLength(maze, true) == 18);
    destroyMaze(maze);
}

void testComputeDistances()
{
    Maze maze = buildMazeFromText(DOOR_MAZE, 10);
    int** distances = createDistanceGrid(maze.size);
    computeDistances(maze, getStartPosition(), false, distances);
    CHECK(distances[0][0] == 0);
    CHECK(distances[0][1] == 1);
    CHECK(distances[1][0] == 1);
    CHECK(distances[1][1] == UNREACHABLE); // wall
    CHECK(distances[0][2] == UNREACHABLE); // closed door
    CHECK(distances[0][3] == UNREACHABLE); // behind the door

    computeDistances(maze, getStartPosition(), true, distances);
    CHECK(distances[0][2] == 2);
    CHECK(distances[9][9] == 18);

    destroyDistanceGrid(distances, maze.size);
    destroyMaze(maze);
}
