#pragma once

#include <stdexcept>

template <typename TType>
class Singleton
{
private:
    static TType* _instance;

    Singleton() = delete;
    ~Singleton() = delete;

public:
    static TType* instance()
    {
        if (!_instance)
            throw std::runtime_error("Instance not created yet.");
        return _instance;
    }

    template<typename ... TArgs>
    static void instantiate(TArgs&& ... p_args)
    {
        if (_instance)
            throw std::runtime_error("Instance already exists.");
        _instance = new TType(std::forward<TArgs>(p_args)...);
    }
};

template <typename TType>
TType* Singleton<TType>::_instance = nullptr;