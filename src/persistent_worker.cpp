#include "persistent_worker.hpp"

PersistentWorker::PersistentWorker() : _thread("Persistent Worker", [this] { workerLoop(); })
{
    _thread.start();
}

PersistentWorker::~PersistentWorker()
{
    {
        std::lock_guard<std::mutex> lock(_mutex);
        _stop = true;
    }
    _cv.notify_one();
    _thread.stop();
}

void PersistentWorker::addTask(const std::string& name, const std::function<void()>& jobToExecute)
{
    {
        std::lock_guard<std::mutex> lock(_mutex);
        _tasks[name] = jobToExecute;
    }
    _cv.notify_one();
}

void PersistentWorker::removeTask(const std::string& name)
{
    std::lock_guard<std::mutex> lock(_mutex);
    _tasks.erase(name);
}

void PersistentWorker::workerLoop()
{
    std::unique_lock<std::mutex> lock(_mutex);
    while (!_stop)
    {
        _cv.wait(lock, [this]{ return _stop || !_tasks.empty(); });

        for (auto& [name, task] : _tasks)
        {
            lock.unlock();
            task();
            lock.lock();
        }
    }
}