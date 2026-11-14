#include "TestFramework.h"

#include <iostream>

static int testsPassed = 0;
static int testsFailed = 0;
static int failedChecksInCurrentTest = 0;

void checkCondition(bool condition, const char* expression, const char* file, int line)
{
    if (!condition)
    {
        ++failedChecksInCurrentTest;
        std::cout << "      FAILED CHECK: " << expression << "  (" << file << ":" << line << ")\n";
    }
}

void runTest(const char* testId, const char* testName, void (*testFunction)())
{
    failedChecksInCurrentTest = 0;
    testFunction();
    if (failedChecksInCurrentTest == 0)
    {
        ++testsPassed;
        std::cout << "[PASS] " << testId << "  " << testName << "\n";
    }
    else
    {
        ++testsFailed;
        std::cout << "[FAIL] " << testId << "  " << testName << "\n";
    }
}

// Prints the totals and returns the process exit code (0 = all tests passed).
int printSummary()
{
    std::cout << "\n----------------------------------------\n"
              << "Total: " << testsPassed + testsFailed << "   Passed: " << testsPassed
              << "   Failed: " << testsFailed << "\n";
    return testsFailed == 0 ? 0 : 1;
}
