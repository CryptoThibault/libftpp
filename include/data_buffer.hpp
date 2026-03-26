#pragma once

#include <vector>
#include <cstddef>

class DataBuffer
{
public:
    template <typename T>
    DataBuffer& operator<<(const T& value);

    template <typename T>
    DataBuffer& operator>>(T& value);

private:
    std::vector<std::byte> data;
    size_t readPos = 0;
};