#pragma once

#include <iostream>
#include <sstream>
#include <mutex>
#include <string>

class ThreadSafeIOStream
{
public:
    template<typename T>
    ThreadSafeIOStream& operator<<(const T& value);

    ThreadSafeIOStream& operator<<(std::ostream& (*manip)(std::ostream&));

    template<typename T>
    ThreadSafeIOStream& operator>>(T& value);

    void setPrefix(const std::string& prefix);
    
    template<typename T>
    void prompt(const std::string& question, T& dest);

private:
    static std::mutex _mutex;
    static thread_local std::string _prefix;
    thread_local static std::ostringstream _oss;
    static thread_local bool _prefixAdded;

    void flushThreadBuffer(std::ostringstream& oss);
};

extern thread_local ThreadSafeIOStream threadSafeCout;

#include "thread_safe_iostream.tpp"