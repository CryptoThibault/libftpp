#include <utility>

template <typename TType>
void Pool<TType>::resize(const size_t& numberOfObjectStored)
{
    _objects.reserve(numberOfObjectStored);
    for (size_t i = 0; i < numberOfObjectStored; ++i)
        _objects.emplace_back();
}

template <typename TType>
template<typename ... TArgs>
typename Pool<TType>::Object& Pool<TType>::acquire(TArgs&& ... p_args)
{
    for (auto& obj : _objects)
    {
        if (!obj._inUse)
        {
            obj.allocate(std::forward<TArgs>(p_args)...);
            return obj;
        }
    }
    throw std::runtime_error("No free objects in the pool");
}

template <typename TType>
Pool<TType>::Object::~Object()
{
    if(_ptr)
    {
        _ptr = nullptr;
        _inUse = false;
    }
}

template <typename TType>
TType* Pool<TType>::Object::operator->()
{
    return _ptr;
}

template <typename TType>
template<typename... TArgs>
void Pool<TType>::Object::allocate(TArgs&&... args)
{
    if (!_ptr)
        _ptr = new TType(std::forward<TArgs>(args)...);
    _inUse = true;
}