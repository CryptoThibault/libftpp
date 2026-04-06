#include "logger.hpp"
#include <thread>
#include <vector>
#include <chrono>

void worker(int id)
{
    for (int i = 0; i < 3; ++i)
    {
        Logger::instance()->info("Worker " + std::to_string(id) + " running");
        std::this_thread::sleep_for(std::chrono::milliseconds(10)); // Simulate work
    }
}

int main()
{
    Logger::instantiate();

    // Logger configuration
    Logger::instance()->setLevel(Level::Debug);
    Logger::instance()->setShowLevel(true);
    Logger::instance()->setShowTime(true);

    // ===== Console test =====
    Logger::instance()->info("=== Console Test Start ===");

    Logger::instance()->trace("Trace message");
    Logger::instance()->debug("Debug message");
    Logger::instance()->info("Info message");
    Logger::instance()->warning("Warning message");
    Logger::instance()->error("Error message");
    Logger::instance()->critical("Critical message");

    // Multi-thread test
    std::vector<std::thread> threads;
    for (int i = 0; i < 3; ++i)
        threads.emplace_back(worker, i);

    for (auto& t : threads)
        t.join();

    Logger::instance()->info("=== Console Test End ===");

    // ===== File test =====
    static std::ofstream file("log.txt");
    Logger::instance()->setOutput(file);

    Logger::instance()->info("=== File Test Start ===");

    Logger::instance()->trace("Trace message");
    Logger::instance()->debug("Debug message");
    Logger::instance()->info("Info message");
    Logger::instance()->warning("Warning message");
    Logger::instance()->error("Error message");
    Logger::instance()->critical("Critical message");

    threads.clear();
    for (int i = 0; i < 3; ++i)
        threads.emplace_back(worker, i);

    for (auto& t : threads)
        t.join();

    Logger::instance()->info("=== File Test End ===");
    Logger::instance()->resetOutput();
    Logger::instance()->info("Back to console");

    return 0;
}