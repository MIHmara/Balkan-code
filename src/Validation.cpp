#include "Validation.h"
#include "GameConfig.h"

// Removes spaces, tabs and line breaks from both ends of the text.
std::string trimWhitespace(const std::string& text)
{
    const char* whitespace = " \t\r\n";
    const std::size_t first = text.find_first_not_of(whitespace);
    if (first == std::string::npos)
    {
        return "";
    }
    const std::size_t last = text.find_last_not_of(whitespace);
    return text.substr(first, last - first + 1);
}

// Converts text to an integer. Accepts an optional + or - sign followed by
// up to 9 digits (so the result always fits in an int). Returns false for
// empty text, letters, decimals or too long numbers.
bool tryParseInt(const std::string& text, int& value)
{
    const std::string trimmed = trimWhitespace(text);
    if (trimmed.empty())
    {
        return false;
    }

    std::size_t index = 0;
    bool isNegative = false;
    if (trimmed[0] == '+' || trimmed[0] == '-')
    {
        isNegative = (trimmed[0] == '-');
        index = 1;
    }

    const std::size_t maxDigits = 9;
    const std::size_t digitCount = trimmed.size() - index;
    if (digitCount == 0 || digitCount > maxDigits)
    {
        return false;
    }

    int result = 0;
    for (; index < trimmed.size(); ++index)
    {
        const char symbol = trimmed[index];
        if (symbol < '0' || symbol > '9')
        {
            return false;
        }
        result = result * 10 + (symbol - '0');
    }

    value = isNegative ? -result : result;
    return true;
}

bool isInRange(int value, int min, int max)
{
    return value >= min && value <= max;
}

bool isValidMazeSize(int size)
{
    return isInRange(size, MIN_MAZE_SIZE, MAX_MAZE_SIZE);
}

bool isValidSeed(int seed)
{
    return isInRange(seed, 0, MAX_SEED);
}
