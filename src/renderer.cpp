#include "Renderer.h"
#include "GameConfig.h"

#include <cstdlib>
#include <iostream>

// Every tile is drawn with two characters so the maze looks square.
const char* const PLAYER_SYMBOL = "@ ";
const char* const FRAME_SYMBOL = "##";

void clearScreen()
{
#ifdef _WIN32
    std::system("cls");
#else
    std::cout << "\033[2J\033[H"; // ANSI: clear screen, cursor to top-left
#endif
}

void showIntro()
{
    std::cout << "=============================================\n"
              << "                 M A Z E\n"
              << "=============================================\n\n"
              << "You wake up in the top-left corner of a dark maze.\n"
              << "The only way out is the exit in the bottom-right corner.\n"
              << "Some walls hide special doors - find a key and take\n"
              << "a shortcut, or walk the long way around.\n";
}

void showMainMenu()
{
    std::cout << "================ MAIN MENU ================\n"
              << "  " << MENU_NEW_GAME << ". New game\n"
              << "  " << MENU_NEW_GAME_WITH_SEED << ". New game with seed (replay a maze)\n"
              << "  " << MENU_HOW_TO_PLAY << ". How to play\n"
              << "  " << MENU_ABOUT << ". About\n"
              << "  " << MENU_EXIT << ". Exit\n"
              << "===========================================\n";
}

void showHelp()
{
    std::cout << "================ HOW TO PLAY ================\n\n"
              << "Goal: get from the start (top-left) to the exit (bottom-right).\n\n"
              << "Controls:\n"
              << "  Arrow keys or W/A/S/D  move up / left / down / right\n"
              << "  I                      show inventory\n"
              << "  L                      show information about your location\n"
              << "  H                      show this help\n"
              << "  Q                      give up and return to the menu\n\n"
              << "Rules:\n"
              << "  - You cannot walk through walls or leave the maze.\n"
              << "  - Keys are picked up automatically when you step on them.\n"
              << "  - A door opens with one key and stays open. Doors are\n"
              << "    shortcuts towards the exit.\n"
              << "  - Par = the shortest route without doors.\n"
              << "    Finish at or under par for 3 stars.\n\n"
              << "Maze elements:\n";
    showTileInfo(Tile::Wall);
    showTileInfo(Tile::Path);
    showTileInfo(Tile::Key);
    showTileInfo(Tile::Door);
    showTileInfo(Tile::Exit);
    std::cout << "  [" << PLAYER_SYMBOL << "] You  - your current position.\n";
}

void showAbout()
{
    std::cout << "================ ABOUT ================\n\n"
              << "Maze size: " << MIN_MAZE_SIZE << "x" << MIN_MAZE_SIZE << " to " << MAX_MAZE_SIZE
              << "x" << MAX_MAZE_SIZE << ", stored in dynamic arrays.\n"
              << "Every game generates a new random maze.\n"
              << "Use the seed shown during the game to replay the same maze.\n";
}

void showGoodbye()
{
    std::cout << "\nThank you for playing. Have a nice day!\n";
}

const char* getTileSymbol(Tile tile)
{
    switch (tile)
    {
    case Tile::Wall: return "##";
    case Tile::Path: return "  ";
    case Tile::Door: return "D ";
    case Tile::Key:  return "k ";
    case Tile::Exit: return "E ";
    }
    return "??";
}

const char* getTileName(Tile tile)
{
    switch (tile)
    {
    case Tile::Wall: return "Wall";
    case Tile::Path: return "Path";
    case Tile::Door: return "Door";
    case Tile::Key:  return "Key";
    case Tile::Exit: return "Exit";
    }
    return "Unknown";
}

const char* getTileDescription(Tile tile)
{
    switch (tile)
    {
    case Tile::Wall: return "solid stone, you cannot pass.";
    case Tile::Path: return "an open corridor.";
    case Tile::Door: return "a special door, opens with a key and shortens the way.";
    case Tile::Key:  return "a key, step on it to pick it up.";
    case Tile::Exit: return "the way out. Reach it to win!";
    }
    return "";
}

const char* getMoveMessage(MoveResult result)
{
    switch (result)
    {
    case MoveResult::Moved:        return "";
    case MoveResult::HitWall:      return "Ouch! A wall blocks the way.";
    case MoveResult::HitEdge:      return "You cannot leave the maze this way.";
    case MoveResult::DoorLocked:   return "The door is locked. Find a key first.";
    case MoveResult::DoorOpened:   return "You used a key and opened the door!";
    case MoveResult::KeyCollected: return "You found a key!";
    case MoveResult::ReachedExit:  return "You reached the exit!";
    case MoveResult::GameOver:     return "The game is over.";
    }
    return "";
}

// Draws the maze top-down inside a frame. The whole picture is built in a
// string first and printed at once to reduce flickering.
void drawMaze(const GameState& game)
{
    const Maze& maze = game.maze;

    std::string frameLine;
    for (int col = 0; col < maze.size + 2; ++col)
    {
        frameLine += FRAME_SYMBOL;
    }

    std::string picture = frameLine + "\n";
    for (int row = 0; row < maze.size; ++row)
    {
        picture += FRAME_SYMBOL;
        for (int col = 0; col < maze.size; ++col)
        {
            const Position pos{row, col};
            if (isSamePosition(pos, game.player.pos))
            {
                picture += PLAYER_SYMBOL;
            }
            else
            {
                picture += getTileSymbol(getTile(maze, pos));
            }
        }
        picture += FRAME_SYMBOL;
        picture += "\n";
    }
    picture += frameLine + "\n";

    std::cout << picture;
}

void showLegend()
{
    std::cout << "Legend: " << PLAYER_SYMBOL << "you  " << getTileSymbol(Tile::Exit) << "exit  "
              << getTileSymbol(Tile::Key) << "key  " << getTileSymbol(Tile::Door) << "door  "
              << getTileSymbol(Tile::Wall) << " wall\n";
}

void showStatus(const GameState& game)
{
    const Player& player = game.player;
    std::cout << "Maze " << game.maze.size << "x" << game.maze.size << " | Seed " << game.seed
              << " | Moves " << player.moves << " (par " << game.parMoves << ")"
              << " | Keys " << player.keys << "\n";
    showLegend();
}

void showMessage(const std::string& message)
{
    std::cout << ">> " << message << "\n";
}

void showCommandPrompt()
{
    std::cout << "[Arrows/WASD] move  [I] inventory  [L] location  [H] help  [Q] quit\n";
    std::cout.flush();
}

void showLocationInfo(const GameState& game)
{
    const Position pos = game.player.pos;
    std::cout << "================ LOCATION ================\n\n"
              << "Position: row " << pos.row << ", column " << pos.col << "\n"
              << "Exit:     row " << game.maze.size - 1 << ", column " << game.maze.size - 1
              << "\n"
              << "Shortest way to the exit (without doors): " << distanceToExit(game)
              << " steps\n\n"
              << "Open directions:";

    const Direction directions[4] = {Direction::Up, Direction::Down, Direction::Left,
                                     Direction::Right};
    const char* names[4] = {" up", " down", " left", " right"};
    bool anyOpen = false;
    for (int index = 0; index < 4; ++index)
    {
        if (canMove(game, directions[index]))
        {
            std::cout << names[index];
            anyOpen = true;
        }
    }
    if (!anyOpen)
    {
        std::cout << " none";
    }
    std::cout << "\n";
}

void showTileInfo(Tile tile)
{
    std::cout << "  [" << getTileSymbol(tile) << "] " << getTileName(tile) << " - "
              << getTileDescription(tile) << "\n";
}

void showInventory(const Player& player)
{
    std::cout << "================ INVENTORY ================\n\n"
              << "Keys in your pocket: " << player.keys << "\n"
              << "Keys found so far:   " << player.keysCollected << "\n"
              << "Doors opened:        " << player.doorsOpened << "\n";
}

void showVictory(const GameState& game)
{
    const Player& player = game.player;
    const int stars = getStarRating(game);

    std::cout << "\n*********** YOU ESCAPED THE MAZE! ***********\n\n"
              << "Moves:           " << player.moves << " (par " << game.parMoves << ")\n"
              << "Bumps into walls: " << player.blockedAttempts << "\n"
              << "Keys found:      " << player.keysCollected << "\n"
              << "Doors opened:    " << player.doorsOpened << "\n"
              << "Rating:          ";
    for (int star = 0; star < 3; ++star)
    {
        std::cout << (star < stars ? "*" : ".");
    }
    std::cout << "\nSeed of this maze: " << game.seed << "\n";
}

void showGameAbandoned(const GameState& game)
{
    std::cout << "You left the maze after " << game.player.moves << " moves.\n"
              << "Use seed " << game.seed << " to try the same maze again.\n";
}
