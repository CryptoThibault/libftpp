#pragma once

#include "data_buffer.hpp"

class Message {
public:
    using Type = int;
    
    Message(int type);

    template<typename TType>
    Message& operator<<(const TType& value)
    {
        _data << value;
        return *this;
    }

    template<typename TType>
    Message& operator>>(TType& value) const
    {
        _data >> value;
        return const_cast<Message&>(*this);
    }

    DataBuffer& data();
    const DataBuffer& data() const;
    int type() const;

private:
    int _type;
    mutable DataBuffer _data;
};