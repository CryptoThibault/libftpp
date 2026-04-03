#pragma once

#include <stdexcept>

template <typename T>
class Singleton
{
public:
    static T* instance()
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
        _instance = new T(std::forward<TArgs>(p_args)...);
    }

private:
    static T* _instance;

    Singleton() = delete;
    ~Singleton() = delete;
};

template <typename T>
T* Singleton<T>::_instance = nullptr;