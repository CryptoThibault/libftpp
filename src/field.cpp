#include "field.hpp"

Field::Field() : _value("") {}
Field::Field(const std::string& s) : _value(s) {}
Field::Field(const char* s) : _value(std::string(s)) {}
Field::Field(double d) : _value(d) {}
Field::Field(float n) : _value(static_cast<double>(n)) {}
Field::Field(int n) : _value(static_cast<double>(n)) {}
Field::Field(long n) : _value(static_cast<double>(n)) {}
Field::Field(bool b) : _value(b) {}
Field::Field(const DataVector& v) : _value(v) {}
Field::Field(const DataMap& m) : _value(m) {}

bool Field::isString() const { return std::holds_alternative<std::string>(_value); }
bool Field::isNumber() const { return std::holds_alternative<double>(_value); }
bool Field::isBool()   const { return std::holds_alternative<bool>(_value); }
bool Field::isVector() const { return std::holds_alternative<DataVector>(_value); }
bool Field::isMap()    const { return std::holds_alternative<DataMap>(_value); }

Field::operator std::string() const { return std::get<std::string>(_value); }
Field::operator double() const { return std::get<double>(_value); }
Field::operator int() const { return static_cast<int>(std::get<double>(_value)); }
Field::operator bool() const { return std::get<bool>(_value); }
Field::operator DataVector() const { return std::get<DataVector>(_value); }
Field::operator DataMap() const { return std::get<DataMap>(_value); }