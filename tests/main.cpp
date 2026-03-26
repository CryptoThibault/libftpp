#include "tester.hpp"

int main()
{
    bool success = true;

    success &= runTest("DATA_STRUCTURES", dataStructuresTest);

    printFinalResult(success);

    return success ? 0 : 1;
}