template <typename T>
T* Pool<T>::Object::operator->() {
    return _ptr;
}

template <typename T>
template<typename... TArgs>
void Pool<T>::Object::allocate(TArgs&&... args)
{
    if (!_ptr)
    {
        _ptr = new T(std::forward<TArgs>(args)...);
    }
    else if constexpr (sizeof...(TArgs) > 0)
    {
        _ptr->~T();
        ::new (_ptr) T(std::forward<TArgs>(args)...);
    }
    _inUse = true;
}

template <typename T>
void Pool<T>::Object::release() {
    if (_ptr)
        _ptr->~T();
    _inUse = false;
}

template <typename T>
Pool<T>::~Pool() {
    for (auto& obj : _objects)
    {
        delete obj._ptr;
        obj._ptr = nullptr;
    }
}

template <typename T>
void Pool<T>::resize(const size_t& numberOfObjectStored) {
    _objects.reserve(numberOfObjectStored);
    size_t currentSize = _objects.size();
    for (size_t i = currentSize; i < numberOfObjectStored; ++i)
    {
        _objects.emplace_back();
        _objects.back()._ptr = new T();
    }
}

template <typename T>
template<typename... TArgs>
typename Pool<T>::Object Pool<T>::acquire(TArgs&&... args) {
    for (auto& obj : _objects)
    {
        if (!obj._inUse)
        {
            if (obj._ptr)
                obj._ptr->~T();
            obj.allocate(std::forward<TArgs>(args)...);
            return obj;
        }
    }
    throw std::runtime_error("No free objects in the pool");
}