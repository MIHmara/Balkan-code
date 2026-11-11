#ifndef RENDERER_H
#define RENDERER_H

#include <string>
#include "Game.h"

// ---------------------------------------------------------------------------
// Renderer module (Frontend): everything that is printed on the screen.
// ---------------------------------------------------------------------------

void clearScreen();
void showIntro();
void showMainMenu();
void showHelp();
void showAbout();
void showGoodbye();

const char* getTileSymbol(Tile tile);
const char* getTileName(Tile tile);
const char* getTileDescription(Tile tile);
const char* getMoveMessage(MoveResult result);

void drawMaze(const GameState& game);
void showLegend();
void showStatus(const GameState& game);
void showMessage(const std::string& message);
void showCommandPrompt();
void showLocationInfo(const GameState& game);
void showTileInfo(Tile tile);
void showInventory(const Player& player);
void showVictory(const GameState& game);
void showGameAbandoned(const GameState& game);

#endif // RENDERER_H