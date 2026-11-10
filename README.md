# Maze

A console maze game written in C++. The player starts in the top-left corner and must
reach the exit in the bottom-right corner. Every game generates a new random maze
from 10x10 up to 50x50 tiles, stored in dynamically allocated arrays.

Some walls are **special doors** that shorten the way to the exit. A door opens with a
**key** that lies somewhere in the maze — the player decides whether the detour for the
key is worth the shortcut.

```
############################
##@ ##      ##            ##
##k ##  ##  ##  ##  ##  ####
##      ##  ##  ##  ##  ####
##D ######  ######  ########
##      ##      ##      ####
##  ##########  ######  ####
##          ##            ##
##  ######  ##########  ####
##  ##  ##              ####
##  ##  ####################
##                        ##
##  ##  ##  ##  ########E ##
############################
Maze 12x12 | Seed 585319 | Moves 0 (par 50) | Keys 0
Legend: @ you  E exit  k key  D door  ## wall
```

## Features

- Maze size chosen by the player: 10x10 to 50x50 (validated input)
- New random maze on every game; replay any maze by its seed
- Movement with the arrow keys (W/A/S/D also work)
- Walls, open corridors, keys, special doors and an exit
- Inventory, location info, in-game help
- Par (shortest route) and a 1–3 star rating at the end
- 34 automated tests

## Controls

| Key | Action |
|---|---|
| Arrow keys / W A S D | Move up / left / down / right |
| I | Inventory |
| L | Location information |
| H | Help |
| Q | Give up and return to the menu |

## Build and run

**Visual Studio 2022 (Windows):** extract the archive, open `Maze.sln`, press
`Ctrl+Shift+B` to build and `Ctrl+F5` to run. The workload *Desktop development with
C++* must be installed. The solution has two projects: `Maze` (the game, startup project)
and `MazeTests` (the automated tests). No .NET is needed – this is native C++.

**g++ / MinGW / Linux / macOS:**

```bash
make          # builds ./maze
./maze
make test     # builds and runs the automated tests
```

**CMake:**

```bash
cmake -S . -B build
cmake --build build
./build/maze
./build/maze_tests
```
