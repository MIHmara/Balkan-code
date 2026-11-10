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
