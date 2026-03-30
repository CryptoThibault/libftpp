#pragma once

#include <vector>
#include <string>
#include <cstddef>
#include <stdexcept>

class DataBuffer
{
public:
    template <typename T>
    DataBuffer& operator<<(const T& value);

    template <typename T>
    DataBuffer& operator>>(T& value);


    DataBuffer& operator<<(const std::string& str);
    DataBuffer& operator>>(std::string& str);

    void clear();
    size_t size() const;

private:
    std::vector<std::byte> data;
    size_t readPos = 0;
};

#include "data_buffer.tpp"