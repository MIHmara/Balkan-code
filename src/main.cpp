// ===========================================================================
// Maze - console game in C++
// Entry point: main menu and game loop. The game logic lives in Game/Maze,
// the screen output in Renderer and the keyboard handling in Input.
// ===========================================================================

#include "../include/Game.h"
#include "../include/GameConfig.h"
#include "../include/Input.h"
#include "../include/Renderer.h"

#include <ctime>
#include <iostream>
#include <random>
#include <string>

// Creates a new random seed for every game.
static unsigned int generateSeed()
{
    std::random_device device;
    const unsigned int mixed = device() ^ static_cast<unsigned int>(std::time(nullptr));
    return mixed % static_cast<unsigned int>(MAX_SEED + 1);
}

// Converts a movement key to a direction. Returns false for other keys.
static bool keyToDirection(Key key, Direction& direction)
{
    switch (key)
    {
    case Key::Up:    direction = Direction::Up;    return true;
    case Key::Down:  direction = Direction::Down;  return true;
    case Key::Left:  direction = Direction::Left;  return true;
    case Key::Right: direction = Direction::Right; return true;
    default:         return false;
    }
}

// Runs one game until the player reaches the exit or quits.
static void playGame(int size, unsigned int seed)
{
    GameState game = startNewGame(size, seed);
    std::string message = "Find the exit in the bottom-right corner!";
    bool hasQuit = false;

    while (!game.finished && !hasQuit)
    {
        clearScreen();
        drawMaze(game);
        showStatus(game);
        showMessage(message);
        showCommandPrompt();

        const Key key = readKey();
        Direction direction = Direction::Up;
        if (keyToDirection(key, direction))
        {
            message = getMoveMessage(movePlayer(game, direction));
            continue;
        }

        switch (key)
        {
        case Key::Help:
            clearScreen();
            showHelp();
            waitForKey();
            break;
        case Key::Inventory:
            clearScreen();
            showInventory(game.player);
            waitForKey();
            break;
        case Key::Location:
            clearScreen();
            showLocationInfo(game);
            waitForKey();
            break;
        case Key::Quit:
            hasQuit = true;
            break;
        default:
            message = "Unknown key. Press H for help.";
            break;
        }
    }

    clearScreen();
    drawMaze(game);
    if (game.finished)
    {
        showVictory(game);
    }
    else
    {
        showGameAbandoned(game);
    }
    waitForKey();
    endGame(game); // release the dynamic memory of the maze
}

// Asks for the maze size. Returns false if the input stream is closed.
static bool askMazeSize(int& size)
{
    const std::string prompt = "Enter maze size (" + std::to_string(MIN_MAZE_SIZE) + "-" +
                               std::to_string(MAX_MAZE_SIZE) + "): ";
    return readIntInRange(prompt, MIN_MAZE_SIZE, MAX_MAZE_SIZE, size);
}

int main()
{
    clearScreen();
    showIntro();
    waitForKey();

    bool isRunning = true;
    while (isRunning)
    {
        clearScreen();
        showMainMenu();

        int choice = MENU_EXIT;
        if (!readIntInRange("Choose an option: ", MENU_EXIT, MENU_ABOUT, choice))
        {
            break;
        }

        int size = MIN_MAZE_SIZE;
        int seed = 0;
        switch (choice)
        {
        case MENU_NEW_GAME:
            if (!askMazeSize(size))
            {
                isRunning = false;
                break;
            }
            playGame(size, generateSeed());
            break;

        case MENU_NEW_GAME_WITH_SEED:
            if (!askMazeSize(size) ||
                !readIntInRange("Enter seed (0-" + std::to_string(MAX_SEED) + "): ", 0, MAX_SEED,
                                seed))
            {
                isRunning = false;
                break;
            }
            playGame(size, static_cast<unsigned int>(seed));
            break;

        case MENU_HOW_TO_PLAY:
            clearScreen();
            showHelp();
            waitForKey();
            break;

        case MENU_ABOUT:
            clearScreen();
            showAbout();
            waitForKey();
            break;

        case MENU_EXIT:
        default:
            isRunning = false;
            break;
        }
    }

    showGoodbye();
    return 0;
}