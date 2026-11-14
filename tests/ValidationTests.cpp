#include "AllTests.h"
#include "GameConfig.h"
#include "Input.h"
#include "TestFramework.h"
#include "Validation.h"

// ---------------------------------------------------------------- TC-V ----

void testParseValidNumbers()
{
    int value = 0;
    CHECK(tryParseInt("10", value) && value == 10);
    CHECK(tryParseInt("  25  ", value) && value == 25);
    CHECK(tryParseInt("+7", value) && value == 7);
    CHECK(tryParseInt("-3", value) && value == -3);
    CHECK(tryParseInt("0", value) && value == 0);
    CHECK(tryParseInt("50\r", value) && value == 50); // Windows line ending
}

void testParseInvalidText()
{
    int value = 123;
    CHECK(!tryParseInt("", value));
    CHECK(!tryParseInt("   ", value));
    CHECK(!tryParseInt("abc", value));
    CHECK(!tryParseInt("12a", value));
    CHECK(!tryParseInt("1.5", value));
    CHECK(!tryParseInt("-", value));
    CHECK(!tryParseInt("+", value));
    CHECK(!tryParseInt("1 2", value));
    CHECK(value == 123); // the output value is not changed on failure
}

void testParseTooLongNumbers()
{
    int value = 0;
    CHECK(tryParseInt("999999999", value) && value == 999999999);
    CHECK(!tryParseInt("1234567890", value));
    CHECK(!tryParseInt("99999999999999999999", value));
}

void testMazeSizeBoundaries()
{
    CHECK(!isValidMazeSize(MIN_MAZE_SIZE - 1));
    CHECK(isValidMazeSize(MIN_MAZE_SIZE));
    CHECK(isValidMazeSize(30));
    CHECK(isValidMazeSize(MAX_MAZE_SIZE));
    CHECK(!isValidMazeSize(MAX_MAZE_SIZE + 1));
    CHECK(!isValidMazeSize(-10));
    CHECK(!isValidMazeSize(0));
}

void testSeedBoundaries()
{
    CHECK(!isValidSeed(-1));
    CHECK(isValidSeed(0));
    CHECK(isValidSeed(MAX_SEED));
    CHECK(!isValidSeed(MAX_SEED + 1));
}

void testTrimWhitespace()
{
    CHECK(trimWhitespace("  a b \r\n") == "a b");
    CHECK(trimWhitespace("\t42") == "42");
    CHECK(trimWhitespace("") == "");
    CHECK(trimWhitespace("   ") == "");
}

// ---------------------------------------------------------------- TC-I ----

void testMovementKeys()
{
    CHECK(keyFromChar('w') == Key::Up && keyFromChar('W') == Key::Up);
    CHECK(keyFromChar('s') == Key::Down && keyFromChar('S') == Key::Down);
    CHECK(keyFromChar('a') == Key::Left && keyFromChar('A') == Key::Left);
    CHECK(keyFromChar('d') == Key::Right && keyFromChar('D') == Key::Right);
}

void testCommandKeys()
{
    CHECK(keyFromChar('h') == Key::Help && keyFromChar('?') == Key::Help);
    CHECK(keyFromChar('i') == Key::Inventory && keyFromChar('I') == Key::Inventory);
    CHECK(keyFromChar('l') == Key::Location && keyFromChar('L') == Key::Location);
    CHECK(keyFromChar('q') == Key::Quit && keyFromChar('Q') == Key::Quit);
}

void testUnknownKeys()
{
    CHECK(keyFromChar('x') == Key::Unknown);
    CHECK(keyFromChar('1') == Key::Unknown);
    CHECK(keyFromChar(' ') == Key::Unknown);
    CHECK(keyFromChar(0) == Key::Unknown);
}
