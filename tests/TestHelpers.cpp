#include "TestHelpers.h"

Maze buildMazeFromText(const char* const rows[], int size)
{
    Maze maze = createMaze(size);
    for (int row = 0; row < size; ++row)
    {
        for (int col = 0; col < size; ++col)
        {
            Tile tile = Tile::Wall;
            switch (rows[row][col])
            {
            case '.': tile = Tile::Path; break;
            case 'D': tile = Tile::Door; break;
            case 'K': tile = Tile::Key;  break;
            case 'E': tile = Tile::Exit; break;
            default:  tile = Tile::Wall; break;
            }
            maze.grid[row][col] = tile;
        }
    }
    return maze;
}

int walkShortestRoute(GameState& game)
{
    const int size = game.maze.size;
    int** fromExit = createDistanceGrid(size);
    computeDistances(game.maze, getExitPosition(game.maze), false, fromExit);

    const Direction directions[4] = {Direction::Up, Direction::Down, Direction::Left,
                                     Direction::Right};
    int movesMade = 0;
    const int safetyLimit = size * size;

    while (!game.finished && movesMade < safetyLimit)
    {
        const Position here = game.player.pos;
        bool stepped = false;
        for (int index = 0; index < 4 && !stepped; ++index)
        {
            const Position next = getNextPosition(here, directions[index]);
            if (isInside(game.maze, next) &&
                fromExit[next.row][next.col] == fromExit[here.row][here.col] - 1)
            {
                movePlayer(game, directions[index]);
                ++movesMade;
                stepped = true;
            }
        }
        if (!stepped)
        {
            break; // no route - the test will detect that the game is not finished
        }
    }

    destroyDistanceGrid(fromExit, size);
    return movesMade;
}
