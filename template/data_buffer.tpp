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
        throw std::out_of_range("Buffer underflow");

    std::memcpy(&value, data.data() + readPos, sizeof(T));
    readPos += sizeof(T);

    return *this;
}

DataBuffer& DataBuffer::operator<<(const std::string& str)
{
    size_t size = str.size();
    *this << size;

    const std::byte* ptr = reinterpret_cast<const std::byte*>(str.data());
    data.insert(data.end(), ptr, ptr + size);

    return *this;
}

DataBuffer& DataBuffer::operator>>(std::string& str)
{
    size_t size;
    *this >> size;

    if (readPos + size > data.size())
        throw std::out_of_range("Buffer underflow");

    str.assign(reinterpret_cast<char*>(data.data() + readPos), size);
    readPos += size;

    return *this;
}