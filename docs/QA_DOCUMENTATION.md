# Maze – QA Documentation

## 1. Scope and approach

| Level | What is tested | How |
|---|---|---|
| Unit tests | Validation, key mapping, maze generation, path finding, game rules | Automated – `tests/` (34 test functions) |
| Integration | Full games on generated mazes from start to exit | Automated – TC-G09 |
| Memory | Leaks and invalid memory access | AddressSanitizer run of tests and game |
| System / UI | Menu, arrow keys, screens, input errors | Manual – section 4 |
| Build | No errors and no warnings | `make` with strict warning flags |

The game logic is separated from the console (no `cin`/`cout` in `Maze`, `Game`,
`Validation`), so it can be tested without a person pressing keys.

## 2. Test framework

`tests/TestFramework.h` is a small framework written for the project – no external
libraries are needed.

| Element | Description |
|---|---|
| `CHECK(condition)` | Records a failed check with the expression, file and line; the test continues |
| `runTest(id, name, function)` | Runs one test function and prints `[PASS]` or `[FAIL]` |
| `printSummary()` | Prints totals; returns 0 if all tests passed, 1 otherwise |

Helpers in `tests/TestHelpers.cpp`:

| Function | Purpose |
|---|---|
| `buildMazeFromText(rows, size)` | Builds a maze from text (`#` wall, `.` path, `D` door, `K` key, `E` exit) for exact, hand-made scenarios |
| `walkShortestRoute(game)` | Plays the shortest route automatically through `movePlayer` |

Hand-made maze used by several tests (door at (0,2), key at (1,0)):

```
..D.......
K########.
#########.
#########.
#########.
#########.
#########.
#########.
#########.
#########E
```

Without the door the exit is unreachable; with the door the route is 18 steps.

### Running the tests

```bash
make test
```

or build the `maze_tests` target in CMake / Visual Studio and run it.

## 3. Automated test cases

### 3.1 Validation – `tests/ValidationTests.cpp`

| ID | Test function | Input / steps | Expected result |
|---|---|---|---|
| TC-V01 | `testParseValidNumbers` | "10", "  25  ", "+7", "-3", "0", "50\r" | Parsed to 10, 25, 7, −3, 0, 50 |
| TC-V02 | `testParseInvalidText` | "", "   ", "abc", "12a", "1.5", "-", "+", "1 2" | Rejected; output value unchanged |
| TC-V03 | `testParseTooLongNumbers` | "999999999", "1234567890", 20 digits | First accepted, others rejected (no overflow) |
| TC-V04 | `testMazeSizeBoundaries` | 9, 10, 30, 50, 51, −10, 0 | Only 10, 30, 50 are valid |
| TC-V05 | `testSeedBoundaries` | −1, 0, 999999, 1000000 | Only 0 and 999999 are valid |
| TC-V06 | `testTrimWhitespace` | "  a b \r\n", "\t42", "", "   " | "a b", "42", "", "" |

### 3.2 Input mapping – `tests/ValidationTests.cpp`

| ID | Test function | Input | Expected result |
|---|---|---|---|
| TC-I01 | `testMovementKeys` | w/W, a/A, s/S, d/D | Up, Left, Down, Right |
| TC-I02 | `testCommandKeys` | h, ?, i/I, l/L, q/Q | Help, Help, Inventory, Location, Quit |
| TC-I03 | `testUnknownKeys` | 'x', '1', ' ', 0 | Unknown |

### 3.3 Maze – `tests/MazeTests.cpp`

| ID | Test function | Steps | Expected result |
|---|---|---|---|
| TC-M01 | `testCreateAndDestroyMaze` | Create 10x10, destroy twice | All tiles walls; after destroy `grid == nullptr`, `size == 0`; second destroy is safe |
| TC-M02 | `testIsInside` | Corners and positions outside | (0,0), (9,9) inside; −1 and 10 outside |
| TC-M03 | `testIsWalkable` | Every tile type, doors closed/open | Wall never; Path, Key, Exit always; Door only when allowed |
| TC-M04 | `testStartAndExitForAllSizes` | Generate every size 10–50 | Start is open, exactly one exit in the corner |
| TC-M05 | `testExitReachableWithoutDoors` | 41 sizes × 6 seeds (246 mazes) | Exit always reachable without keys |
| TC-M06 | `testAllOpenTilesConnected` | Sizes 10, 15 … 50 | Every open tile reachable from the start |
| TC-M07 | `testSameSeedSameMaze` | Two 30x30 mazes, seed 12345 | Identical |
| TC-M08 | `testDifferentSeedsDifferentMazes` | Seeds 1 and 2 | Different |
| TC-M09 | `testDoorsArePlacedCorrectly` | Every size 10–50 | At most size/10 doors; each door between two open tiles |
| TC-M10 | `testDoorsShortenTheRoute` | Every size 10–50 | Route with doors is shorter than without; doors appear at least once |
| TC-M11 | `testKeysArePlacedCorrectly` | Every size 10–50 | Keys = doors; no key on start/exit; all keys reachable |
| TC-M12 | `testShortestPathOnHandMadeMaze` | Hand-made maze | Without doors: unreachable; with doors: 18 |
| TC-M13 | `testComputeDistances` | BFS from start on hand-made maze | Start 0, neighbours 1, walls and closed doors −1; exit 18 with doors open |

### 3.4 Game – `tests/GameTests.cpp`

| ID | Test function | Steps | Expected result |
|---|---|---|---|
| TC-G01 | `testNewGameInitialState` | `startNewGame(20, 42)` | Player at (0,0), 0 moves, 0 keys, par > 0, not finished |
| TC-G02 | `testMoveBlockedByEdge` | Up and Left from start | `HitEdge`; position and moves unchanged; 2 blocked attempts |
| TC-G03 | `testMoveBlockedByWall` | Right, then Down into a wall | `Moved`, then `HitWall`; stays at (0,1) |
| TC-G04 | `testDoorLockedWithoutKey` | Right twice | `DoorLocked`; door still closed |
| TC-G05 | `testKeyIsCollected` | Down onto the key | `KeyCollected`; 1 key; tile becomes path |
| TC-G06 | `testDoorOpensWithKey` | Take key, return, go through door | `DoorOpened`; 0 keys; 1 door opened; door becomes path |
| TC-G07 | `testReachingExitWinsTheGame` | Full route through the door | `ReachedExit`; finished; 20 moves; next move `GameOver` and not counted |
| TC-G08 | `testCanMove` | Edge, path, key, wall, door with/without key | Correct true/false for each |
| TC-G09 | `testShortestRouteWinsGeneratedMazes` | Play shortest route on 10x10, 25x25, 50x50 | Game won in exactly par moves; 3 stars |
| TC-G10 | `testStarRating` | Par 20; moves 18, 20, 30, 31 | 3, 3, 2, 1 stars |
| TC-G11 | `testDistanceToExitEqualsPar` | New 35x35 game | Distance at start = par |
| TC-G12 | `testEndGameReleasesMemory` | `endGame` | `grid == nullptr`, `size == 0` |

## 4. Manual test cases

| ID | Area | Steps | Expected result |
|---|---|---|---|
| MT-01 | Menu | Enter `abc`, `7`, empty line, `1.5` | Error message, question repeated, no crash |
| MT-02 | Size | Choose 1, enter `9`, then `51`, then `10` | 9 and 51 rejected with range message; 10x10 maze shown |
| MT-03 | Size | Start games with 10, 11, 49 and 50 | Correct size; odd and even sizes look complete |
| MT-04 | Arrows | Press ↑ ↓ ← → | Player moves one tile; maze redrawn after every move |
| MT-05 | WASD | Press W A S D (also with Caps Lock) | Same as arrows |
| MT-06 | Walls | Walk into a wall and into the border | Message shown; position and move counter unchanged |
| MT-07 | Keys | Step on `k` | Message "You found a key!"; Keys counter +1; `k` disappears |
| MT-08 | Doors | Walk into `D` without a key, then with a key | Locked message, then door opens and stays open |
| MT-09 | Exit | Reach `E` | Victory screen: moves, par, rating, seed |
| MT-10 | Commands | Press I, L, H during the game | Matching screen; any key returns to the game |
| MT-11 | Unknown key | Press `x`, `5`, F1 | "Unknown key" message, no move |
| MT-12 | Quit | Press Q during a game | Seed shown, return to menu |
| MT-13 | Seed | Note the seed, choose menu 2 with the same size and seed | Exactly the same maze |
| MT-14 | Random | Start two new games with the same size | Different mazes |
| MT-15 | Display | Play 50x50 in a console at least 104 characters wide | Whole maze visible, no broken lines |
| MT-16 | Exit | Choose 0 in the menu | Goodbye message, program ends |
| MT-17 | Platforms | Run on Windows (Visual Studio) and Linux/macOS | Arrows work on both |

## 5. Test results

| Date | Build | Automated | Manual | Notes |
|---|---|---|---|---|
| _dd.mm.2026_ | _commit_ | 34 / 34 passed | _x / 17 passed_ | |

Additional checks on the reference build:

- g++ 13 with `-Wall -Wextra -Wpedantic -Wshadow -Wconversion`: 0 errors, 0 warnings
- AddressSanitizer + UndefinedBehaviorSanitizer on tests and a played game: no findings

## 6. Bug report template

Bugs are reported as GitHub Issues with the label `bug`.

```
Title:        [Area] Short description
Steps:        1. ... 2. ... 3. ...
Expected:     ...
Actual:       ...
Maze size / seed:  ...
System:       Windows / Linux / macOS, compiler
Severity:     Critical / Major / Minor
```
