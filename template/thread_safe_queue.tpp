#include "thread_safe_queue.hpp"

template <typename TType>
void ThreadSafeQueue<TType>::push_back(const TType& newElement)
{
    std::lock_guard<std::mutex> lock(_mutex);
    _queue.push_back(newElement);
}

template <typename TType>
void ThreadSafeQueue<TType>::push_front(const TType& newElement)
{
    std::lock_guard<std::mutex> lock(_mutex);
    _queue.push_front(newElement);
}

template <typename TType>
TType ThreadSafeQueue<TType>::pop_back()
{
    std::lock_guard<std::mutex> lock(_mutex);
    if (_queue.empty()) throw std::runtime_error("Queue is empty");
    TType element = std::move(_queue.back());
    _queue.pop_back();
    return element;
}

template <typename TType>
TType ThreadSafeQueue<TType>::pop_front()
{
    std::lock_guard<std::mutex> lock(_mutex);
    if (_queue.empty()) throw std::runtime_error("Queue is empty");
    TType element = std::move(_queue.front());
    _queue.pop_front();
    return element;
}

template <typename TType>
bool ThreadSafeQueue<TType>::empty() const
{
    std::lock_guard<std::mutex> lock(_mutex);
    return _queue.empty();
}

template <typename TType>
size_t ThreadSafeQueue<TType>::size() const
{
    std::lock_guard<std::mutex> lock(_mutex);
    return _queue.size();
}