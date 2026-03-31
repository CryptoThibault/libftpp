#pragma once

#include <functional>
#include <vector>
#include <memory>
#include <mutex>
#include <condition_variable>
#include "thread_safe_queue.hpp"
#include "thread.hpp"

class WorkerPool
{
public:
    WorkerPool(size_t numThreads);
    ~WorkerPool();

    void addJob(const std::function<void()>& jobToExecute);

    class IJobs
    {
    public:
        virtual ~IJobs() = default;
        virtual void execute() = 0;
    };

private:
    class FunctionJob : public IJobs
    {
    public:
        FunctionJob(std::function<void()> f) : _func(std::move(f)) {}
        void execute() override { _func(); }

    private:
        std::function<void()> _func;
    };

    std::vector<Thread> _threads;
    ThreadSafeQueue<std::shared_ptr<IJobs>> _jobs;
    std::mutex _mutex;
    std::condition_variable _cv;
    bool _stop = false;

    void workerLoop();
};