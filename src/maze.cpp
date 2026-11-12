#include "Maze.h"

// ===========================================================================
// Memory management
// ===========================================================================

// Allocates a size x size maze where every tile is a wall.
Maze createMaze(int size)
{
    Maze maze;
    maze.size = size;
    maze.grid = new Tile*[size];
    for (int row = 0; row < size; ++row)
    {
        maze.grid[row] = new Tile[size];
    }
    fillWithWalls(maze);
    return maze;
}

// Releases the memory of the maze. Safe to call on an already destroyed maze.
void destroyMaze(Maze& maze)
{
    if (maze.grid == nullptr)
    {
        return;
    }
    for (int row = 0; row < maze.size; ++row)
    {
        delete[] maze.grid[row];
    }
    delete[] maze.grid;
    maze.grid = nullptr;
    maze.size = 0;
}

// ===========================================================================
// Basic queries
// ===========================================================================

// The player always starts in the top-left corner.
Position getStartPosition()
{
    return Position{0, 0};
}

// The exit is always in the bottom-right corner.
Position getExitPosition(const Maze& maze)
{
    return Position{maze.size - 1, maze.size - 1};
}

bool isInside(const Maze& maze, Position pos)
{
    return pos.row >= 0 && pos.row < maze.size && pos.col >= 0 && pos.col < maze.size;
}

bool isSamePosition(Position first, Position second)
{
    return first.row == second.row && first.col == second.col;
}

// Walls are never walkable; doors are walkable only when allowDoors is true.
bool isWalkable(Tile tile, bool allowDoors)
{
    switch (tile)
    {
    case Tile::Wall:
        return false;
    case Tile::Door:
        return allowDoors;
    case Tile::Path:
    case Tile::Key:
    case Tile::Exit:
        return true;
    }
    return false;
}

Tile getTile(const Maze& maze, Position pos)
{
    return maze.grid[pos.row][pos.col];
}

void setTile(Maze& maze, Position pos, Tile tile)
{
    maze.grid[pos.row][pos.col] = tile;
}

int countTiles(const Maze& maze, Tile tile)
{
    int count = 0;
    for (int row = 0; row < maze.size; ++row)
    {
        for (int col = 0; col < maze.size; ++col)
        {
            if (maze.grid[row][col] == tile)
            {
                ++count;
            }
        }
    }
    return count;
}

// ===========================================================================
// Generation
// ===========================================================================

void fillWithWalls(Maze& maze)
{
    for (int row = 0; row < maze.size; ++row)
    {
        for (int col = 0; col < maze.size; ++col)
        {
            maze.grid[row][col] = Tile::Wall;
        }
    }
}

// Randomized depth-first search ("recursive backtracker"), implemented with an
// explicit dynamic stack instead of recursion.
// Tiles with even row and even column are "rooms". The algorithm walks from
// room to room (two tiles at a time) and removes the wall between them.
// The result is a "perfect" maze: every open tile is reachable and there is
// exactly one route between any two tiles.
void carvePassages(Maze& maze, std::mt19937& rng)
{
    const int roomsPerSide = (maze.size + 1) / 2;
    const int stackCapacity = roomsPerSide * roomsPerSide; // every room is pushed once
    int* stackRows = new int[stackCapacity];
    int* stackCols = new int[stackCapacity];
    int top = 0;

    const int stepRow[4] = {-2, 2, 0, 0};
    const int stepCol[4] = {0, 0, -2, 2};

    maze.grid[0][0] = Tile::Path;
    stackRows[top] = 0;
    stackCols[top] = 0;
    ++top;

    while (top > 0)
    {
        const int row = stackRows[top - 1];
        const int col = stackCols[top - 1];

        // Collect the directions that lead to rooms which are not visited yet.
        int candidates[4];
        int candidateCount = 0;
        for (int dir = 0; dir < 4; ++dir)
        {
            const Position next{row + stepRow[dir], col + stepCol[dir]};
            if (isInside(maze, next) && getTile(maze, next) == Tile::Wall)
            {
                candidates[candidateCount] = dir;
                ++candidateCount;
            }
        }

        if (candidateCount == 0)
        {
            --top; // dead end - go back one room
            continue;
        }

        std::uniform_int_distribution<int> pick(0, candidateCount - 1);
        const int dir = candidates[pick(rng)];
        const int nextRow = row + stepRow[dir];
        const int nextCol = col + stepCol[dir];

        maze.grid[row + stepRow[dir] / 2][col + stepCol[dir] / 2] = Tile::Path; // remove the wall
        maze.grid[nextRow][nextCol] = Tile::Path;

        stackRows[top] = nextRow;
        stackCols[top] = nextCol;
        ++top;
    }

    delete[] stackRows;
    delete[] stackCols;
}

// Marks the bottom-right corner as the exit. For even sizes the corner is not
// a "room", so it is connected to the nearest room above it.
void openExit(Maze& maze)
{
    const int last = maze.size - 1;
    if (last % 2 == 1)
    {
        maze.grid[last - 1][last] = Tile::Path;
    }
    maze.grid[last][last] = Tile::Exit;
}

// For even sizes the last row and the last column are not part of the room
// grid. Some of their tiles are opened as small dead ends, so the right and
// bottom borders look like the rest of the maze. No loops are created.
void openBorderNooks(Maze& maze, std::mt19937& rng)
{
    const int last = maze.size - 1;
    if (last % 2 == 0)
    {
        return; // odd size - the room grid already reaches the border
    }
    std::bernoulli_distribution shouldOpen(0.5);
    for (int index = 0; index < last - 1; index += 2)
    {
        if (shouldOpen(rng))
        {
            maze.grid[index][last] = Tile::Path; // right column, next to room (index, last - 1)
        }
        if (shouldOpen(rng))
        {
            maze.grid[last][index] = Tile::Path; // bottom row, next to room (last - 1, index)
        }
    }
}

// Returns true if there is a door in the 3x3 area around pos.
static bool hasDoorNearby(const Maze& maze, Position pos)
{
    for (int dRow = -1; dRow <= 1; ++dRow)
    {
        for (int dCol = -1; dCol <= 1; ++dCol)
        {
            const Position near{pos.row + dRow, pos.col + dCol};
            if (isInside(maze, near) && getTile(maze, near) == Tile::Door)
            {
                return true;
            }
        }
    }
    return false;
}

// How many steps would be saved if the wall at pos became a passage.
// The wall must separate two reachable tiles (up/down or left/right).
static int calculateDoorSaving(const Maze& maze, Position pos, int** fromStart, int** fromExit,
                               int shortest)
{
    const Position pairs[2][2] = {
        {{pos.row - 1, pos.col}, {pos.row + 1, pos.col}},
        {{pos.row, pos.col - 1}, {pos.row, pos.col + 1}}};

    int bestSaving = 0;
    for (int pair = 0; pair < 2; ++pair)
    {
        const Position first = pairs[pair][0];
        const Position second = pairs[pair][1];
        if (!isInside(maze, first) || !isInside(maze, second))
        {
            continue;
        }
        if (fromStart[first.row][first.col] == UNREACHABLE ||
            fromStart[second.row][second.col] == UNREACHABLE)
        {
            continue;
        }
        const int viaFirst = fromStart[first.row][first.col] + fromExit[second.row][second.col];
        const int viaSecond = fromStart[second.row][second.col] + fromExit[first.row][first.col];
        const int newLength = (viaFirst < viaSecond ? viaFirst : viaSecond) + 2;
        const int saving = shortest - newLength;
        if (saving > bestSaving)
        {
            bestSaving = saving;
        }
    }
    return bestSaving;
}

// Turns up to maxDoors walls into special doors. Each door is placed where it
// shortens the way to the exit the most (at least by minSaving steps).
// Returns the number of doors placed.
int placeDoors(Maze& maze, int maxDoors, int minSaving)
{
    const int size = maze.size;
    int** fromStart = createDistanceGrid(size);
    int** fromExit = createDistanceGrid(size);
    computeDistances(maze, getStartPosition(), false, fromStart);
    computeDistances(maze, getExitPosition(maze), false, fromExit);

    const int shortest = fromStart[size - 1][size - 1];
    int placed = 0;

    while (placed < maxDoors && shortest != UNREACHABLE)
    {
        int bestSaving = minSaving - 1;
        Position best{-1, -1};

        for (int row = 0; row < size; ++row)
        {
            for (int col = 0; col < size; ++col)
            {
                const Position pos{row, col};
                if (getTile(maze, pos) != Tile::Wall || hasDoorNearby(maze, pos))
                {
                    continue;
                }
                const int saving = calculateDoorSaving(maze, pos, fromStart, fromExit, shortest);
                if (saving > bestSaving)
                {
                    bestSaving = saving;
                    best = pos;
                }
            }
        }

        if (best.row < 0)
        {
            break; // no more useful places for doors
        }
        setTile(maze, best, Tile::Door);
        ++placed;
    }

    destroyDistanceGrid(fromStart, size);
    destroyDistanceGrid(fromExit, size);
    return placed;
}

// Distance from the start to the nearer side of a door (doors closed).
static int getDoorEntryDistance(const Maze& maze, Position door, int** fromStart)
{
    const Position neighbours[4] = {{door.row - 1, door.col},
                                    {door.row + 1, door.col},
                                    {door.row, door.col - 1},
                                    {door.row, door.col + 1}};
    int entry = UNREACHABLE;
    for (int index = 0; index < 4; ++index)
    {
        const Position near = neighbours[index];
        if (!isInside(maze, near))
        {
            continue;
        }
        const int distance = fromStart[near.row][near.col];
        if (distance != UNREACHABLE && (entry == UNREACHABLE || distance < entry))
        {
            entry = distance;
        }
    }
    return entry;
}

// Places one key for every door. The key is put on a random open tile that is
// at most half as far from the start as the door itself, so going for the key
// and then through the door is a real shortcut option for the player.
// Keys are never placed on the start or the exit. Returns the number of keys.
int placeKeys(Maze& maze, std::mt19937& rng)
{
    const int size = maze.size;
    int** fromStart = createDistanceGrid(size);
    computeDistances(maze, getStartPosition(), false, fromStart);

    int* candidates = new int[size * size]; // tile index = row * size + col
    int placed = 0;

    for (int row = 0; row < size; ++row)
    {
        for (int col = 0; col < size; ++col)
        {
            const Position door{row, col};
            if (getTile(maze, door) != Tile::Door)
            {
                continue;
            }
            const int limit = getDoorEntryDistance(maze, door, fromStart) / 2;

            // First choice: tiles close to the start. Fallback: any open tile.
            int candidateCount = 0;
            for (int pass = 0; pass < 2 && candidateCount == 0; ++pass)
            {
                for (int keyRow = 0; keyRow < size; ++keyRow)
                {
                    for (int keyCol = 0; keyCol < size; ++keyCol)
                    {
                        const int distance = fromStart[keyRow][keyCol];
                        const bool isOpen = maze.grid[keyRow][keyCol] == Tile::Path;
                        const bool isClose = (pass == 1) || distance <= limit;
                        if (isOpen && distance > 0 && isClose)
                        {
                            candidates[candidateCount] = keyRow * size + keyCol;
                            ++candidateCount;
                        }
                    }
                }
            }
            if (candidateCount == 0)
            {
                continue;
            }

            std::uniform_int_distribution<int> pick(0, candidateCount - 1);
            const int index = candidates[pick(rng)];
            maze.grid[index / size][index % size] = Tile::Key;
            ++placed;
        }
    }

    delete[] candidates;
    destroyDistanceGrid(fromStart, size);
    return placed;
}

// Builds a complete random maze. The same seed always gives the same maze.
// Doors: up to 1 per 10 rows (1 door for 10x10 ... 5 doors for 50x50).
// One key is placed for every door.
void generateMaze(Maze& maze, unsigned int seed)
{
    std::mt19937 rng(seed);

    fillWithWalls(maze);
    carvePassages(maze, rng);
    openBorderNooks(maze, rng);
    openExit(maze);

    const int maxDoors = maze.size / 10;
    const int minSaving = maze.size / 2;
    placeDoors(maze, maxDoors, minSaving);
    placeKeys(maze, rng);
}

// ===========================================================================
// Path finding (Breadth-First Search)
// ===========================================================================

int** createDistanceGrid(int size)
{
    int** distances = new int*[size];
    for (int row = 0; row < size; ++row)
    {
        distances[row] = new int[size];
    }
    return distances;
}

void destroyDistanceGrid(int** distances, int size)
{
    if (distances == nullptr)
    {
        return;
    }
    for (int row = 0; row < size; ++row)
    {
        delete[] distances[row];
    }
    delete[] distances;
}

// Fills distances with the minimal number of steps from source to every tile.
// Tiles that cannot be reached get UNREACHABLE.
void computeDistances(const Maze& maze, Position source, bool allowDoors, int** distances)
{
    const int size = maze.size;
    for (int row = 0; row < size; ++row)
    {
        for (int col = 0; col < size; ++col)
        {
            distances[row][col] = UNREACHABLE;
        }
    }
    if (!isInside(maze, source) || !isWalkable(getTile(maze, source), allowDoors))
    {
        return;
    }

    const int stepRow[4] = {-1, 1, 0, 0};
    const int stepCol[4] = {0, 0, -1, 1};

    int* queue = new int[size * size]; // each tile enters the queue at most once
    int head = 0;
    int tail = 0;

    distances[source.row][source.col] = 0;
    queue[tail] = source.row * size + source.col;
    ++tail;

    while (head < tail)
    {
        const int index = queue[head];
        ++head;
        const int row = index / size;
        const int col = index % size;

        for (int dir = 0; dir < 4; ++dir)
        {
            const Position next{row + stepRow[dir], col + stepCol[dir]};
            if (!isInside(maze, next) || !isWalkable(getTile(maze, next), allowDoors))
            {
                continue;
            }
            if (distances[next.row][next.col] != UNREACHABLE)
            {
                continue; // already visited
            }
            distances[next.row][next.col] = distances[row][col] + 1;
            queue[tail] = next.row * size + next.col;
            ++tail;
        }
    }

    delete[] queue;
}

// Number of moves on the shortest route from the start to the exit.
int shortestPathLength(const Maze& maze, bool allowDoors)
{
    int** distances = createDistanceGrid(maze.size);
    computeDistances(maze, getStartPosition(), allowDoors, distances);
    const Position exit = getExitPosition(maze);
    const int length = distances[exit.row][exit.col];
    destroyDistanceGrid(distances, maze.size);
    return length;
}
