#pragma once

#include <cstddef>
#include <vector>
#include <stdexcept>

template <typename TType>
class Pool
{
public:
    void resize(const size_t& numberOfObjectStored);

    template<typename ... TArgs>
    typename Pool<TType>::Object& acquire(TArgs&& ... p_args);

    class Object
    {
    public:
        ~Object();

        TType* operator->();

    private:
        TType* _ptr = nullptr;
        bool _inUse = false;

        template<typename ... TArgs>
        void allocate(TArgs&& ... args);

        friend class Pool<TType>;
    };

private:
    std::vector<Object> _objects;
};

#include "pool.tpp"