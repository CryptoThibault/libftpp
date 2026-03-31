template<typename T>
ThreadSafeIOStream& ThreadSafeIOStream::operator<<(const T& value)
{
    if (!_prefixAdded)
    {
        _oss << _prefix;
        _prefixAdded = true;
    }

    _oss << value;

    if constexpr (std::is_same_v<T, std::string>)
        if (!value.empty() && value.back() == '\n')
        {
            std::lock_guard<std::mutex> lock(_mutex);
            std::cout << _oss.str();
            _oss.str("");
            _oss.clear();
            _prefixAdded = false;
        }

    return *this;
}

template<typename T>
ThreadSafeIOStream& ThreadSafeIOStream::operator>>(T& value)
{
    std::lock_guard<std::mutex> lock(_mutex);
    std::cin >> value;
    return *this;
}

template<typename T>
void ThreadSafeIOStream::prompt(const std::string& question, T& dest)
{
    std::lock_guard<std::mutex> lock(_mutex);
    std::cout << _prefix << question;
    std::cin >> dest;
}