#pragma once 
#include <string>
#include <vector>
#include <map>
#include <variant>
#include <stdexcept>

class Field;
using DataVector = std::vector<Field>;
using DataMap    = std::map<std::string, Field>;

class Field {
public:
    using Variant = std::variant<std::string, double, bool, DataVector, DataMap>;

    Field();
    Field(const std::string& s);
    Field(const char* s);
    Field(double n);
    Field(float n);
    Field(int n);
    Field(long n);
    Field(bool b);
    Field(const DataVector& v);
    Field(const DataMap& m);

    bool isString() const;
    bool isNumber() const;
    bool isBool() const;
    bool isVector() const;
    bool isMap() const;

    operator std::string() const;
    operator double() const;
    operator float() const;
    operator int() const;
    operator long() const;
    operator bool() const;
    operator DataVector() const;
    operator DataMap() const;

    template<typename T>
    std::vector<T> asVectorOf() const;

    template<typename T>
    std::map<std::string, T> asMapOf() const;

private:
    Variant _value;
};

#include "field.tpp"