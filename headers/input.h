#ifndef INPUT_H
#define INPUT_H

#include <string>

// ---------------------------------------------------------------------------
// Input module (Frontend): keyboard handling for Windows and Linux/macOS.
// ---------------------------------------------------------------------------

// Logical keys used by the game, independent of the operating system.
enum class Key
{
    Up,
    Down,
    Left,
    Right,
    Help,
    Inventory,
    Location,
    Quit,
    Unknown
};

Key keyFromChar(int character);
Key readKey();
void waitForKey();
bool readIntInRange(const std::string& prompt, int min, int max, int& value);

#endif // INPUT_H