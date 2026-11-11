#ifndef GAME_CONFIG_H
#define GAME_CONFIG_H

// ---------------------------------------------------------------------------
// Global configuration constants shared by all modules.
// ---------------------------------------------------------------------------

// The maze is always square (size x size tiles). The player chooses the size.
const int MIN_MAZE_SIZE = 10;
const int MAX_MAZE_SIZE = 50;

// Seeds are limited to 6 digits so the player can easily retype one
// and replay exactly the same maze.
const int MAX_SEED = 999999;

// Main menu options.
const int MENU_EXIT = 0;
const int MENU_NEW_GAME = 1;
const int MENU_NEW_GAME_WITH_SEED = 2;
const int MENU_HOW_TO_PLAY = 3;
const int MENU_ABOUT = 4;

#endif // GAME_CONFIG_H