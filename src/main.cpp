#include "Input.h"
#include "Validation.h"

#include <cstdio>
#include <iostream>

#ifdef _WIN32
#include <conio.h>
#else
#include <termios.h>
#include <unistd.h>
#endif

// Maps an ordinary character to a game key (letters are case-insensitive).
// W/A/S/D work as an alternative to the arrow keys.
Key keyFromChar(int character)
{
    switch (character)
    {
    case 'w': case 'W':
        return Key::Up;
    case 's': case 'S':
        return Key::Down;
    case 'a': case 'A':
        return Key::Left;
    case 'd': case 'D':
        return Key::Right;
    case 'h': case 'H': case '?':
        return Key::Help;
    case 'i': case 'I':
        return Key::Inventory;
    case 'l': case 'L':
        return Key::Location;
    case 'q': case 'Q':
        return Key::Quit;
    default:
        return Key::Unknown;
    }
}

#ifndef _WIN32
// Reads one character immediately, without waiting for Enter and without
// printing it (Linux/macOS terminal "raw" mode).
static int readRawChar()
{
    termios oldSettings;
    if (tcgetattr(STDIN_FILENO, &oldSettings) != 0)
    {
        return std::getchar(); // input is not a terminal (e.g. redirected file)
    }
    termios rawSettings = oldSettings;
    rawSettings.c_lflag &= ~(ICANON | ECHO);
    rawSettings.c_cc[VMIN] = 1;
    rawSettings.c_cc[VTIME] = 0;
    tcsetattr(STDIN_FILENO, TCSANOW, &rawSettings);
    const int character = std::getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldSettings);
    return character;
}
#endif

// Waits for one key press and converts it to a game key.
// Arrow keys send special codes that differ between operating systems:
//   Windows:      0 or 224, followed by 72 / 80 / 75 / 77
//   Linux/macOS:  ESC [ A / B / D / C
Key readKey()
{
#ifdef _WIN32
    const int character = _getch();
    if (character == 0 || character == 224)
    {
        switch (_getch())
        {
        case 72: return Key::Up;
        case 80: return Key::Down;
        case 75: return Key::Left;
        case 77: return Key::Right;
        default: return Key::Unknown;
        }
    }
    return keyFromChar(character);
#else
    const int character = readRawChar();
    if (character == EOF)
    {
        return Key::Quit; // no more input - leave the game safely
    }
    if (character == 27 && readRawChar() == '[')
    {
        switch (readRawChar())
        {
        case 'A': return Key::Up;
        case 'B': return Key::Down;
        case 'C': return Key::Right;
        case 'D': return Key::Left;
        default: return Key::Unknown;
        }
    }
    return keyFromChar(character);
#endif
}

void waitForKey()
{
    std::cout << "\nPress any key to continue...";
    std::cout.flush();
    readKey();
}

// Asks repeatedly until the user enters a whole number between min and max.
// Returns false only when the input stream is closed.
bool readIntInRange(const std::string& prompt, int min, int max, int& value)
{
    while (true)
    {
        std::cout << prompt;
        std::string line;
        if (!std::getline(std::cin, line))
        {
            return false;
        }

        int parsed = 0;
        if (!tryParseInt(line, parsed))
        {
            std::cout << "  Invalid input. Please enter a whole number.\n";
            continue;
        }
        if (!isInRange(parsed, min, max))
        {
            std::cout << "  Out of range. Please enter a number from " << min << " to " << max
                      << ".\n";
            continue;
        }
        value = parsed;
        return true;
    }
}
