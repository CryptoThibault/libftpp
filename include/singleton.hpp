#pragma once

#include <stdexcept>

template <typename TType>
class Singleton
{
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

private:
    static TType* _instance;

    Singleton() = delete;
    ~Singleton() = delete;
};

template <typename TType>
TType* Singleton<TType>::_instance = nullptr;