#include "thread_safe_iostream.hpp"

std::mutex ThreadSafeIOStream::_mutex;
thread_local std::string ThreadSafeIOStream::_prefix;
thread_local std::ostringstream ThreadSafeIOStream::_oss;
thread_local bool ThreadSafeIOStream::_prefixAdded = false;
thread_local ThreadSafeIOStream threadSafeCout;

ThreadSafeIOStream& ThreadSafeIOStream::operator<<(std::ostream& (*manip)(std::ostream&))
{
    if (manip == static_cast<std::ostream& (*)(std::ostream&)>(std::endl))
    {
        std::lock_guard<std::mutex> lock(_mutex);
        std::cout << _oss.str() << std::endl;
        _oss.str("");
        _oss.clear();
        _prefixAdded = false;
    }
    else
        _oss << manip;

    return *this;
}

void ThreadSafeIOStream::setPrefix(const std::string& prefix)
{
    _prefix = prefix;
}

void ThreadSafeIOStream::setOutput(std::ostream& stream)
{
    std::lock_guard<std::mutex> lock(_mutex);
    std::cout.rdbuf(stream.rdbuf());
}