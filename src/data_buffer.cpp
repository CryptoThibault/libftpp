#include "data_buffer.hpp"
#include <cstring>
#include <stdexcept>

template <typename T>
DataBuffer& DataBuffer::operator<<(const T& value)
{
    const std::byte* raw = reinterpret_cast<const std::byte*>(&value);
    data.insert(data.end(), raw, raw + sizeof(T));
    return *this;
}

template <typename T>
DataBuffer& DataBuffer::operator>>(T& value)
{
    if (readPos + sizeof(T) > data.size())
        throw std::out_of_range("Trying to read past buffer");

    std::memcpy(&value, data.data() + readPos, sizeof(T));
    readPos += sizeof(T);
    return *this;
}