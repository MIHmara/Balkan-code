// ===========================================================================
// Test runner: executes every automated test and prints a summary.
// Exit code 0 means all tests passed.
// ===========================================================================

#include "AllTests.h"
#include "TestFramework.h"

#include <iostream>

int main()
{
    std::cout << "Maze - automated tests\n\n";

    std::cout << "Validation\n";
    runTest("TC-V01", "Parse valid numbers", testParseValidNumbers);
    runTest("TC-V02", "Reject invalid text", testParseInvalidText);
    runTest("TC-V03", "Reject too long numbers", testParseTooLongNumbers);
    runTest("TC-V04", "Maze size boundaries 10-50", testMazeSizeBoundaries);
    runTest("TC-V05", "Seed boundaries", testSeedBoundaries);
    runTest("TC-V06", "Trim whitespace", testTrimWhitespace);

    std::cout << "\nInput mapping\n";
    runTest("TC-I01", "Movement keys W/A/S/D", testMovementKeys);
    runTest("TC-I02", "Command keys H/I/L/Q", testCommandKeys);
    runTest("TC-I03", "Unknown keys", testUnknownKeys);

    std::cout << "\nMaze\n";
    runTest("TC-M01", "Create and destroy maze", testCreateAndDestroyMaze);
    runTest("TC-M02", "Inside / outside the maze", testIsInside);
    runTest("TC-M03", "Walkable tiles", testIsWalkable);
    runTest("TC-M04", "Start and exit for sizes 10-50", testStartAndExitForAllSizes);
    runTest("TC-M05", "Exit reachable without doors", testExitReachableWithoutDoors);
    runTest("TC-M06", "All open tiles connected", testAllOpenTilesConnected);
    runTest("TC-M07", "Same seed gives same maze", testSameSeedSameMaze);
    runTest("TC-M08", "Different seeds give different mazes", testDifferentSeedsDifferentMazes);
    runTest("TC-M09", "Doors placed correctly", testDoorsArePlacedCorrectly);
    runTest("TC-M10", "Doors shorten the route", testDoorsShortenTheRoute);
    runTest("TC-M11", "Keys placed correctly", testKeysArePlacedCorrectly);
    runTest("TC-M12", "Shortest path on hand-made maze", testShortestPathOnHandMadeMaze);
    runTest("TC-M13", "Distance calculation (BFS)", testComputeDistances);

    std::cout << "\nGame\n";
    runTest("TC-G01", "New game initial state", testNewGameInitialState);
    runTest("TC-G02", "Move blocked by edge", testMoveBlockedByEdge);
    runTest("TC-G03", "Move blocked by wall", testMoveBlockedByWall);
    runTest("TC-G04", "Door locked without key", testDoorLockedWithoutKey);
    runTest("TC-G05", "Key is collected", testKeyIsCollected);
    runTest("TC-G06", "Door opens with key", testDoorOpensWithKey);
    runTest("TC-G07", "Reaching the exit wins", testReachingExitWinsTheGame);
    runTest("TC-G08", "canMove reflects the rules", testCanMove);
    runTest("TC-G09", "Shortest route wins generated mazes", testShortestRouteWinsGeneratedMazes);
    runTest("TC-G10", "Star rating", testStarRating);
    runTest("TC-G11", "Distance to exit equals par", testDistanceToExitEqualsPar);
    runTest("TC-G12", "endGame releases memory", testEndGameReleasesMemory);

    return printSummary();
}
