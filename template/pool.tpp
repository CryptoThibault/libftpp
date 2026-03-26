#include <utility>

template <typename TType>
void Pool<TType>::resize(const size_t& numberOfObjectStored)
{
    for (size_t i = 0; i < numberOfObjectStored; ++i)
        objects.push_back(Object());
}

template <typename TType>
template<typename ... TArgs>
typename Pool<TType>::Object& Pool<TType>::acquire(TArgs&& ... p_args)
{
    for (auto& obj : objects)
    {
        if (!obj.inUse)
        {
            obj.allocate(std::forward<TArgs>(p_args)...);
            return obj;
        }
    }
    throw std::runtime_error("No free objects in the pool");
}

template <typename TType>
Pool<TType>::Object::~Object() noexcept
{
    if(ptr)
    {
        delete ptr;
        ptr = nullptr;
        inUse = false;
    }
}

template <typename TType>
TType* Pool<TType>::Object::operator->() noexcept
{
    return ptr;
}

template <typename TType>
template<typename... TArgs>
void Pool<TType>::Object::allocate(TArgs&&... args)
{
    if (!ptr)
        ptr = new TType(std::forward<TArgs>(args)...);
    inUse = true;
}