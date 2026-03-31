#pragma once

#include <deque>
#include <mutex>
#include <stdexcept>

template <typename TType>
class ThreadSafeQueue
{
public:
    void push_back(const TType& newElement);
    void push_front(const TType& newElement);
    TType pop_back();
    TType pop_front();

    bool empty() const;
    size_t size() const;

private:
    std::deque<TType> _queue;
    mutable std::mutex _mutex;
};

#include "thread_safe_queue.tpp"