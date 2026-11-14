# Build the game and the tests with g++ (Linux, macOS, MinGW on Windows).
#   make          build the game  -> ./maze
#   make test     build and run the automated tests
#   make clean    remove the built files

CXX      ?= g++
CXXFLAGS := -std=c++17 -Wall -Wextra -Wpedantic -Wshadow -Wconversion -Iinclude

LOGIC_SRC := src/Maze.cpp src/Game.cpp src/Validation.cpp src/Input.cpp
GAME_SRC  := $(LOGIC_SRC) src/Renderer.cpp src/main.cpp
TEST_SRC  := $(LOGIC_SRC) $(wildcard tests/*.cpp)

all: maze

maze: $(GAME_SRC)
	$(CXX) $(CXXFLAGS) $^ -o $@

maze_tests: $(TEST_SRC)
	$(CXX) $(CXXFLAGS) -Itests $^ -o $@

test: maze_tests
	./maze_tests

clean:
	rm -f maze maze_tests maze.exe maze_tests.exe

.PHONY: all test clean
