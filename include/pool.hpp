#pragma once

#include <cstddef>
#include <vector>
#include <stdexcept>

template <typename T>
class Pool
{
public:
    void resize(const size_t& numberOfObjectStored);

    template<typename ... TArgs>
    typename Pool<T>::Object& acquire(TArgs&& ... args);

    class Object
    {
    public:
        ~Object();

        T* operator->();

    private:
        T* _ptr = nullptr;
        bool _inUse = false;

        template<typename ... TArgs>
        void allocate(TArgs&& ... args);

        friend class Pool<T>;
    };

private:
    std::vector<Object> _objects;
};

#include <utility>
#include "pool.tpp"