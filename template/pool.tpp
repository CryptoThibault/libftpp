template <typename T>
void Pool<T>::resize(const size_t& numberOfObjectStored)
{
    _objects.reserve(numberOfObjectStored);
    for (size_t i = 0; i < numberOfObjectStored; ++i)
        _objects.emplace_back();
}

template <typename T>
template<typename ... TArgs>
typename Pool<T>::Object& Pool<T>::acquire(TArgs&& ... args)
{
    for (auto& obj : _objects)
    {
        if (!obj._inUse)
        {
            obj.allocate(std::forward<TArgs>(args)...);
            return obj;
        }
    }
    throw std::runtime_error("No free objects in the pool");
}

template <typename T>
Pool<T>::Object::~Object()
{
    if(_ptr)
    {
        _ptr = nullptr;
        _inUse = false;
    }
}

template <typename T>
T* Pool<T>::Object::operator->()
{
    return _ptr;
}

template <typename T>
template<typename... TArgs>
void Pool<T>::Object::allocate(TArgs&&... args)
{
    if (!_ptr)
        _ptr = new T(std::forward<TArgs>(args)...);
    _inUse = true;
}