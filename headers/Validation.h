#ifndef VALIDATION_H
#define VALIDATION_H

#include <string>

// ---------------------------------------------------------------------------
// Validation module (Backend): pure functions that check user input.
// They do not read or print anything, so they are easy to unit test.
// ---------------------------------------------------------------------------

std::string trimWhitespace(const std::string& text);
bool tryParseInt(const std::string& text, int& value);
bool isInRange(int value, int min, int max);
bool isValidMazeSize(int size);
bool isValidSeed(int seed);

#endif // VALIDATION_H
