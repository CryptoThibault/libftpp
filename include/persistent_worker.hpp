#pragma once

#include <string>
#include <functional>
#include <atomic>
#include <map>
#include <mutex>
#include <condition_variable>
#include "thread.hpp"

class PersistentWorker
{
public:
    PersistentWorker();
    ~PersistentWorker();

    void addTask(const std::string& name, const std::function<void()>& jobToExecute);
    void removeTask(const std::string& name);

private:
    Thread _thread;
    std::atomic<bool> _stop{false};
    std::map<std::string, std::function<void()>> _tasks;
    std::mutex _mutex;
    std::condition_variable _cv;

    void workerLoop();
};