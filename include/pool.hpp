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
        ~Object() noexcept;

        TType* operator->() noexcept;

        template<typename... TArgs>
        void allocate(TArgs&&... args);

    private:
        TType* ptr = nullptr;
        bool inUse = false;
        friend class Pool<TType>;
    };

private:
    std::vector<Object> objects;
};

#include "pool.tpp"