#ifndef TEST_FRAMEWORK_H
#define TEST_FRAMEWORK_H

// ---------------------------------------------------------------------------
// Minimal unit-test framework (QA). No external libraries are needed.
// CHECK(condition) records a failed check with file and line, but the test
// continues, so one run shows every problem.
// ---------------------------------------------------------------------------

#define CHECK(condition) checkCondition((condition), #condition, __FILE__, __LINE__)

void checkCondition(bool condition, const char* expression, const char* file, int line);
void runTest(const char* testId, const char* testName, void (*testFunction)());
int printSummary();

#endif // TEST_FRAMEWORK_H
