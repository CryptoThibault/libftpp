#pragma once
#include "field.hpp"
#include <string>

class Loader {
public:
    static DataMap load(const std::string& filepath);
private:
    static DataMap loadMap(const std::string& s, size_t& i);
    static DataVector loadVector(const std::string& s, size_t& i);
    static Field loadField(const std::string& s, size_t& i);
    static void skipWhitespace(const std::string& s, size_t& i);
};