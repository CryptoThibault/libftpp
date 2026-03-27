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