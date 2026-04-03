#pragma once

#include "data_buffer.hpp"

class Memento {
public:
    class Snapshot
    {
    public:
        Snapshot() = default;

        template<typename T>
        Snapshot& operator<<(const T& value)
        {
            write(value);
            return *this;
        }

        template<typename T>
        Snapshot& operator>>(T& value)
        {
            read(value);
            return *this;
        }

    private:
        DataBuffer buffer;

        template<typename T>
        void write(const T& value)
        {
            buffer << value;
        }

        template<typename T>
        void read(T& value)
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