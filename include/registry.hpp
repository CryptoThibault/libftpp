#include <vector>
#include <mutex>
#include <stdexcept>
#include <functional>
#include "singleton.hpp"

template <typename T>
class Registry : public Singleton<Registry<T>>
{
    friend class Singleton<Registry<T>>;

public:
    void add(T obj);
    void remove(const std::function<bool(const T&)>& predicate);
    void clear();

    T* get(size_t index);
    std::vector<T> getAll() const;
    T* find(const std::function<bool(const T&)>& predicate);
    size_t size() const;

private:
    mutable std::mutex _mutex;
    std::vector<T> _objects;
};

#include "registry.tpp"