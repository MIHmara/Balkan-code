# Maze

A console maze game written in C++. The player starts in the top-left corner and must
reach the exit in the bottom-right corner. Every game generates a random maze from
10x10 up to 50x50 tiles, stored in dynamically allocated arrays.

Some walls are **special doors** that shorten the way to the exit. A door opens with a
**key** found somewhere in the maze; the player decides whether the detour for the key
is worth the shortcut.

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
- Inventory, location info and in-game help
- Par (shortest route) and a 1-to-3-star rating at the end

## Controls

| Key | Action |
|---|---|
| Arrow keys / W A S D | Move up / left / down / right |
| I | Inventory |
| L | Location information |
| H | Help |
| Q | Give up and return to the menu |

## Build and run

Install CMake (3.16 or newer) and a C++11-compatible compiler. From the repository root:

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build --config Debug
./build/maze
```

On Windows, configure with the Visual Studio 2022 generator and build from PowerShell:

```powershell
cmake -S . -B build -G "Visual Studio 17 2022" -A x64
cmake --build build --config Debug
.\build\Debug\maze.exe
```

The Visual Studio *Desktop development with C++* workload is required.

## Debug in VS Code (Windows)

Install the Microsoft C/C++ extension, CMake, and the Visual Studio 2022 C++ workload.
Open this folder in VS Code, choose **Debug Maze (Windows)** in Run and Debug, and press
`F5`. The pre-launch task configures and builds the Debug target before launching it.
