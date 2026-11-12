#include "Game.h"

// Prepares a game for an already generated maze. The game takes ownership
// of the maze memory (it is released in endGame()).
void initGame(GameState& game, Maze maze, unsigned int seed)
{
    game.maze = maze;
    game.player = Player();
    game.player.pos = getStartPosition();
    game.seed = seed;
    game.finished = false;

    // "Par" is the shortest route without doors. If the exit can only be
    // reached through a door (possible in hand-made test mazes), doors count.
    game.parMoves = shortestPathLength(maze, false);
    if (game.parMoves == UNREACHABLE)
    {
        game.parMoves = shortestPathLength(maze, true);
    }
}

GameState startNewGame(int size, unsigned int seed)
{
    Maze maze = createMaze(size);
    generateMaze(maze, seed);

    GameState game;
    initGame(game, maze, seed);
    return game;
}

void endGame(GameState& game)
{
    destroyMaze(game.maze);
    game.finished = true;
}

Position getNextPosition(Position from, Direction direction)
{
    switch (direction)
    {
    case Direction::Up:
        return Position{from.row - 1, from.col};
    case Direction::Down:
        return Position{from.row + 1, from.col};
    case Direction::Left:
        return Position{from.row, from.col - 1};
    case Direction::Right:
        return Position{from.row, from.col + 1};
    }
    return from;
}

// True if a move in this direction would succeed right now
// (a door counts as open only if the player has a key).
bool canMove(const GameState& game, Direction direction)
{
    const Position next = getNextPosition(game.player.pos, direction);
    if (!isInside(game.maze, next))
    {
        return false;
    }
    const Tile tile = getTile(game.maze, next);
    if (tile == Tile::Door)
    {
        return game.player.keys > 0;
    }
    return isWalkable(tile, false);
}

// Applies the game rules for one move and returns what happened.
MoveResult movePlayer(GameState& game, Direction direction)
{
    if (game.finished)
    {
        return MoveResult::GameOver;
    }

    Player& player = game.player;
    const Position next = getNextPosition(player.pos, direction);

    if (!isInside(game.maze, next))
    {
        ++player.blockedAttempts;
        return MoveResult::HitEdge;
    }

    MoveResult result = MoveResult::Moved;
    switch (getTile(game.maze, next))
    {
    case Tile::Wall:
        ++player.blockedAttempts;
        return MoveResult::HitWall;

    case Tile::Door:
        if (player.keys == 0)
        {
            ++player.blockedAttempts;
            return MoveResult::DoorLocked;
        }
        --player.keys;
        ++player.doorsOpened;
        setTile(game.maze, next, Tile::Path); // the door stays open
        result = MoveResult::DoorOpened;
        break;

    case Tile::Key:
        ++player.keys;
        ++player.keysCollected;
        setTile(game.maze, next, Tile::Path); // the key is picked up
        result = MoveResult::KeyCollected;
        break;

    case Tile::Exit:
        game.finished = true;
        result = MoveResult::ReachedExit;
        break;

    case Tile::Path:
        break;
    }

    player.pos = next;
    ++player.moves;
    return result;
}

bool isAtExit(const GameState& game)
{
    return isSamePosition(game.player.pos, getExitPosition(game.maze));
}

// Steps left on the shortest route from the player to the exit (doors closed).
int distanceToExit(const GameState& game)
{
    int** distances = createDistanceGrid(game.maze.size);
    computeDistances(game.maze, getExitPosition(game.maze), false, distances);
    const int distance = distances[game.player.pos.row][game.player.pos.col];
    destroyDistanceGrid(distances, game.maze.size);
    return distance;
}

// 3 stars: at or under par, 2 stars: up to 50% over par, 1 star: otherwise.
int getStarRating(const GameState& game)
{
    const int moves = game.player.moves;
    const int par = game.parMoves;
    if (moves <= par)
    {
        return 3;
    }
    if (moves * 2 <= par * 3)
    {
        return 2;
    }
    return 1;
}
