#pragma once

#include <cstddef>
#include <vector>
#include <stdexcept>
#include <utility>

template <typename T>
class Pool {
public:
    class Object {
    public:
        Object(const Object&) = delete;
        Object& operator=(const Object&) = delete;
        
        T* operator->();

        template<typename... TArgs>
        void allocate(TArgs&&... args);

        void release();
        bool inUse() const { return _inUse; }

    private:
        T* _ptr{};
        bool _inUse{};

        friend class Pool<T>;
    };

    ~Pool();

    void resize(const size_t& numberOfObjectStored);

    template<typename... TArgs>
    Object acquire(TArgs&&... args);

private:
    std::vector<Object> _objects;
};

#include "pool.tpp"