cmake_minimum_required(VERSION 3.10)
project(Maze CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -Wpedantic -Wshadow -Wconversion)
endif()

set(LOGIC_SOURCES
    src/Maze.cpp
    src/Game.cpp
    src/Validation.cpp
    src/Input.cpp)

# The game
add_executable(maze ${LOGIC_SOURCES} src/Renderer.cpp src/main.cpp)
target_include_directories(maze PRIVATE include)

# The automated tests
add_executable(maze_tests ${LOGIC_SOURCES}
    tests/TestFramework.cpp
    tests/TestHelpers.cpp
    tests/ValidationTests.cpp
    tests/MazeTests.cpp
    tests/GameTests.cpp
    tests/TestMain.cpp)
target_include_directories(maze_tests PRIVATE include tests)

enable_testing()
add_test(NAME all_tests COMMAND maze_tests)
