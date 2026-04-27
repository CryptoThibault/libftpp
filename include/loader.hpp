#pragma once
#include "field.hpp"
#include <string>

class Loader {
public:
    static FieldMap load(const std::string& filepath);
private:
    static FieldMap loadMap(const std::string& s, size_t& i);
    static FieldVector loadVector(const std::string& s, size_t& i);
    static Field loadField(const std::string& s, size_t& i);
    static void skipWhitespace(const std::string& s, size_t& i);
};