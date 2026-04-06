#include "logger.hpp"
#include "thread_safe_iostream.hpp"
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>

void Logger::log(Level level, const std::string& message)
{
    if (level < _currentLevel) return;

    if (_showTime) threadSafeCout << "[" << getCurrentTime() << "] ";
    if (_showLevel) threadSafeCout << "[" << toString(level) << "] ";
    threadSafeCout << message << std::endl;
}

void Logger::trace(const std::string& message)
{
    log(Level::Trace, message);
}

void Logger::debug(const std::string& message)
{
    log(Level::Debug, message);
}

void Logger::info(const std::string& message)
{
    log(Level::Info, message);
}

void Logger::warning(const std::string& message)
{
    log(Level::Warning, message);
}

void Logger::error(const std::string& message)
{
    log(Level::Error, message);
}

void Logger::critical(const std::string& message)
{
    log(Level::Critical, message);
}

void Logger::setLevel(Level level)
{
    _currentLevel = level;
}

void Logger::setShowLevel(bool show)
{
    _showLevel = show;
}

void Logger::setShowTime(bool show)
{
    _showTime = show;
}

bool Logger::setOutput(std::ofstream& output)
{
    if (!output.is_open()) return false;
    threadSafeCout.setOutput(output);
    return true;
}

void Logger::resetOutput()
{
    if (_output.is_open()) _output.close();
    threadSafeCout.setOutput(std::cout);
}

std::string Logger::toString(Level level)
{
    switch(level)
    {
        case Level::Debug: return "DEBUG";
        case Level::Info: return "INFO";
        case Level::Warning: return "WARNING";
        case Level::Error: return "ERROR";
        case Level::Critical: return "CRITICAL";
        default: return "UNKNOWN";
    }
}

std::string Logger::getCurrentTime() const
{
    static std::mutex timeMutex;

    auto now = std::chrono::system_clock::now();
    auto timeT = std::chrono::system_clock::to_time_t(now);

    std::tm localTime{};
    {
        std::lock_guard<std::mutex> lock(timeMutex);
        localTime = *std::localtime(&timeT);
    }

    std::ostringstream oss;
    oss << std::put_time(&localTime, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}