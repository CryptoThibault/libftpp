#include "data_buffer.hpp"

void DataBuffer::clear()
{
    data.clear();
    readPos = 0;
}

size_t DataBuffer::size() const
{
    return data.size();
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