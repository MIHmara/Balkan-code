#include "AllTests.h"
#include "Game.h"
#include "TestFramework.h"
#include "TestHelpers.h"

// Same layout as in MazeTests: door at (0,2), key at (1,0), exit at (9,9).
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

static GameState createHandMadeGame()
{
    GameState game;
    initGame(game, buildMazeFromText(DOOR_MAZE, 10), 0);
    return game;
}

// ---------------------------------------------------------------- TC-G ----

void testNewGameInitialState()
{
    GameState game = startNewGame(20, 42);
    CHECK(game.maze.size == 20);
    CHECK(game.player.pos.row == 0 && game.player.pos.col == 0);
    CHECK(game.player.moves == 0);
    CHECK(game.player.keys == 0);
    CHECK(game.seed == 42);
    CHECK(game.parMoves > 0);
    CHECK(!game.finished);
    endGame(game);
}

void testMoveBlockedByEdge()
{
    GameState game = createHandMadeGame();
    CHECK(movePlayer(game, Direction::Up) == MoveResult::HitEdge);
    CHECK(movePlayer(game, Direction::Left) == MoveResult::HitEdge);
    CHECK(game.player.pos.row == 0 && game.player.pos.col == 0);
    CHECK(game.player.moves == 0);
    CHECK(game.player.blockedAttempts == 2);
    endGame(game);
}

void testMoveBlockedByWall()
{
    GameState game = createHandMadeGame();
    CHECK(movePlayer(game, Direction::Right) == MoveResult::Moved); // (0,1)
    CHECK(movePlayer(game, Direction::Down) == MoveResult::HitWall); // (1,1) is a wall
    CHECK(game.player.pos.row == 0 && game.player.pos.col == 1);
    CHECK(game.player.moves == 1);
    endGame(game);
}

void testDoorLockedWithoutKey()
{
    GameState game = createHandMadeGame();
    movePlayer(game, Direction::Right);
    CHECK(movePlayer(game, Direction::Right) == MoveResult::DoorLocked);
    CHECK(game.player.pos.col == 1);
    CHECK(getTile(game.maze, Position{0, 2}) == Tile::Door);
    endGame(game);
}

void testKeyIsCollected()
{
    GameState game = createHandMadeGame();
    CHECK(movePlayer(game, Direction::Down) == MoveResult::KeyCollected);
    CHECK(game.player.keys == 1);
    CHECK(game.player.keysCollected == 1);
    CHECK(getTile(game.maze, Position{1, 0}) == Tile::Path); // the key is removed
    endGame(game);
}

void testDoorOpensWithKey()
{
    GameState game = createHandMadeGame();
    movePlayer(game, Direction::Down);  // take the key
    movePlayer(game, Direction::Up);    // back to start
    movePlayer(game, Direction::Right); // (0,1)
    CHECK(movePlayer(game, Direction::Right) == MoveResult::DoorOpened);
    CHECK(game.player.pos.col == 2);
    CHECK(game.player.keys == 0);
    CHECK(game.player.doorsOpened == 1);
    CHECK(getTile(game.maze, Position{0, 2}) == Tile::Path); // the door stays open
    endGame(game);
}

void testReachingExitWinsTheGame()
{
    GameState game = createHandMadeGame();
    movePlayer(game, Direction::Down);
    movePlayer(game, Direction::Up);
    for (int step = 0; step < 9; ++step)
    {
        movePlayer(game, Direction::Right); // through the door to (0,9)
    }
    MoveResult last = MoveResult::Moved;
    for (int step = 0; step < 9; ++step)
    {
        last = movePlayer(game, Direction::Down); // to (9,9)
    }
    CHECK(last == MoveResult::ReachedExit);
    CHECK(game.finished);
    CHECK(isAtExit(game));
    CHECK(game.player.moves == 20);
    CHECK(movePlayer(game, Direction::Up) == MoveResult::GameOver);
    CHECK(game.player.moves == 20);
    endGame(game);
}

void testCanMove()
{
    GameState game = createHandMadeGame();
    CHECK(!canMove(game, Direction::Up));   // edge
    CHECK(canMove(game, Direction::Right)); // path
    CHECK(canMove(game, Direction::Down));  // key
    movePlayer(game, Direction::Right);
    CHECK(!canMove(game, Direction::Down));  // wall
    CHECK(!canMove(game, Direction::Right)); // door, no key
    game.player.keys = 1;
    CHECK(canMove(game, Direction::Right)); // door, with key
    endGame(game);
}

void testShortestRouteWinsGeneratedMazes()
{
    const int sizes[3] = {10, 25, 50};
    for (int index = 0; index < 3; ++index)
    {
        GameState game = startNewGame(sizes[index], 2026);
        const int moves = walkShortestRoute(game);
        CHECK(game.finished);
        CHECK(moves == game.parMoves);
        CHECK(game.player.moves == game.parMoves);
        CHECK(getStarRating(game) == 3);
        endGame(game);
    }
}

void testStarRating()
{
    GameState game = createHandMadeGame();
    game.parMoves = 20;
    game.player.moves = 18;
    CHECK(getStarRating(game) == 3);
    game.player.moves = 20;
    CHECK(getStarRating(game) == 3);
    game.player.moves = 30;
    CHECK(getStarRating(game) == 2);
    game.player.moves = 31;
    CHECK(getStarRating(game) == 1);
    endGame(game);
}

void testDistanceToExitEqualsPar()
{
    GameState game = startNewGame(35, 7);
    CHECK(distanceToExit(game) == game.parMoves);
    endGame(game);
}

void testEndGameReleasesMemory()
{
    GameState game = startNewGame(10, 1);
    CHECK(game.maze.grid != nullptr);
    endGame(game);
    CHECK(game.maze.grid == nullptr);
    CHECK(game.maze.size == 0);
}
