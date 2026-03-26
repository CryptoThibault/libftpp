#include "tester.hpp"
#include <iostream>

constexpr auto RESET  = "\033[0m";
constexpr auto RED    = "\033[31m";
constexpr auto GREEN  = "\033[32m";
constexpr auto BLUE   = "\033[34m";
constexpr auto CYAN   = "\033[36m";

bool runTest(const std::string& name, bool (*testFunc)())
{
    bool result = testFunc();
    printTestResult(name, result);
    return result;
}

void printTestResult(const std::string& name, bool result)
{
    std::cout << CYAN + std::string("[") + name + "] " + RESET
        + (result ? std::string(GREEN) + "OK" + RESET : std::string(RED) + "FAIL" + RESET)
        << std::endl;
}

void printFinalResult(bool success)
{
    std::cout << "\n" << BLUE << "==== FINAL RESULT ====" << RESET << std::endl;
    std::cout << (success ? std::string(GREEN) + "SUCCESS" + RESET
        : std::string(RED) + "FAILURE" + RESET) << std::endl;
}