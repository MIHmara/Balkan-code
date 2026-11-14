#ifndef ALL_TESTS_H
#define ALL_TESTS_H

// Declarations of every test function. IDs match docs/QA_DOCUMENTATION.md.

// Validation (TC-V)
void testParseValidNumbers();
void testParseInvalidText();
void testParseTooLongNumbers();
void testMazeSizeBoundaries();
void testSeedBoundaries();
void testTrimWhitespace();

// Input mapping (TC-I)
void testMovementKeys();
void testCommandKeys();
void testUnknownKeys();

// Maze (TC-M)
void testCreateAndDestroyMaze();
void testIsInside();
void testIsWalkable();
void testStartAndExitForAllSizes();
void testExitReachableWithoutDoors();
void testAllOpenTilesConnected();
void testSameSeedSameMaze();
void testDifferentSeedsDifferentMazes();
void testDoorsArePlacedCorrectly();
void testDoorsShortenTheRoute();
void testKeysArePlacedCorrectly();
void testShortestPathOnHandMadeMaze();
void testComputeDistances();

// Game (TC-G)
void testNewGameInitialState();
void testMoveBlockedByEdge();
void testMoveBlockedByWall();
void testDoorLockedWithoutKey();
void testKeyIsCollected();
void testDoorOpensWithKey();
void testReachingExitWinsTheGame();
void testCanMove();
void testShortestRouteWinsGeneratedMazes();
void testStarRating();
void testDistanceToExitEqualsPar();
void testEndGameReleasesMemory();

#endif // ALL_TESTS_H
