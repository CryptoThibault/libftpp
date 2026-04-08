#include "loader.hpp"
#include <fstream>
#include <sstream>
#include <cctype>
#include <stdexcept>

DataMap Loader::load(const std::string& filepath)
{
    std::ifstream file(filepath);
    if (!file.is_open())
        throw std::runtime_error("Could not open file: " + filepath);

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string s = buffer.str();
    size_t i = 0;
    return loadMap(s, i);
}

DataMap Loader::loadMap(const std::string& s, size_t& i)
{
    DataMap map;
    skipWhitespace(s, i);
    if (s[i] != '{') throw std::runtime_error("Expected '{' at position " + std::to_string(i));
    i++;

    while (true)
    {
        skipWhitespace(s, i);
        if (i >= s.size()) throw std::runtime_error("Unexpected end of object");

        if (s[i] == '}') { i++; break; }

        Field keyField = loadField(s, i);
        std::string key = keyField;

        skipWhitespace(s, i);
        if (s[i] != ':') throw std::runtime_error("Expected ':' after key");
        i++;

        Field value = loadField(s, i);
        map.emplace(key, value);

        skipWhitespace(s, i);
        if (i < s.size() && s[i] == ',') i++;
    }

    return map;
}

DataVector Loader::loadVector(const std::string& s, size_t& i)
{
    DataVector vec;
    skipWhitespace(s, i);
    if (s[i] != '[') throw std::runtime_error("Expected '[' at position " + std::to_string(i));
    i++;

    while (true)
    {
        skipWhitespace(s, i);
        if (i >= s.size()) throw std::runtime_error("Unexpected end of array");

        if (s[i] == ']') { i++; break; }

        vec.push_back(loadField(s, i));

        skipWhitespace(s, i);
        if (i < s.size() && s[i] == ',') i++;
    }

    return vec;
}

Field Loader::loadField(const std::string& s, size_t& i)
{
    skipWhitespace(s, i);
    if (i >= s.size()) throw std::runtime_error("Unexpected end of input");

    if (s[i] == '"')
    {
        std::string str;
        i++;
        while (i < s.size() && s[i] != '"')
        {
            if (s[i] == '\\' && i + 1 < s.size()) { str += s[++i]; }
            else str += s[i];
            i++;
        }
        if (i >= s.size()) throw std::runtime_error("Unterminated string");
        i++;
        return str;
    }
    else if (std::isdigit(s[i]) || s[i] == '-')
    {
        size_t start = i;
        if (s[i] == '-') i++;
        while (i < s.size() && std::isdigit(s[i])) i++;
        return std::stod(s.substr(start, i - start));
    }
    else if (s.compare(i, 4, "true") == 0) { i += 4; return true; }
    else if (s.compare(i, 5, "false") == 0) { i += 5; return false; }
    else if (s[i] == '{') return loadMap(s, i);
    else if (s[i] == '[') return loadVector(s, i);

    throw std::runtime_error("Unexpected character at position " + std::to_string(i));
}

void Loader::skipWhitespace(const std::string& s, size_t& i)
{
    while (i < s.size() && std::isspace(s[i])) i++;
}