#pragma once
#include <string>

bool dataStructuresTest();

bool runTest(const std::string& name, bool (*testFunc)());
void printTestResult(const std::string& name, bool result);
void printFinalResult(bool success);