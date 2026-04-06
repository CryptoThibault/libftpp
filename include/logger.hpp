#pragma once

#include <string>
#include <fstream>
#include "singleton.hpp"

enum class Level
{
    Trace,
    Debug,
    Info,
    Warning,
    Error,
    Critical
};

class Logger : public Singleton<Logger>
{
public:
    void log(Level level, const std::string& message);

    void trace(const std::string& message);
    void debug(const std::string& message);
    void info(const std::string& message);
    void warning(const std::string& message);
    void error(const std::string& message);
    void critical(const std::string& message);

    void setLevel(Level level);
    void setShowLevel(bool show);
    void setShowTime(bool show);

    bool setOutput(std::ofstream& output);
    void resetOutput();

    std::string toString(Level level);
    std::string getCurrentTime() const;

private:
    Level _currentLevel{Level::Info};
    bool _showLevel{};
    bool _showTime{};
    std::ofstream _output;

    friend class Singleton<Logger>;
};