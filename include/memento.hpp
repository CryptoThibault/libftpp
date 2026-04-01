#pragma once

#include "data_buffer.hpp"

class Memento {
public:
    class Snapshot
    {
    public:
        Snapshot() = default;

        template<typename TType>
        Snapshot& operator<<(const TType& value)
        {
            write(value);
            return *this;
        }

        template<typename TType>
        Snapshot& operator>>(TType& value)
        {
            read(value);
            return *this;
        }

    private:
        DataBuffer buffer;

        template<typename TType>
        void write(const TType& value)
        {
            buffer << value;
        }

        template<typename TType>
        void read(TType& value)
        {
            buffer >> value;
        }

        friend class Memento;
    };

    Snapshot save();
    void load(const Memento::Snapshot& state);

private:
    virtual void _saveToSnapshot(Snapshot& snapshot) const = 0;
    virtual void _loadFromSnapshot(Snapshot& snapshot) = 0;
};