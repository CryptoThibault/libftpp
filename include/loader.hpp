#pragma once

#include <string>
#include <vector>
#include <map>
#include <variant>
#include <fstream>
#include "json.hpp"

class Loader {
public:
    struct ObjectMap;
    using DataVector = std::vector<ObjectMap>;
    using Field      = std::variant<std::string, DataVector>;
    struct ObjectMap : std::map<std::string, Field> {};
    using DataMap = std::map<std::string, DataVector>; 

    static DataMap load(const std::string& filepath);

private:
    static DataVector composeDataVector(const nlohmann::json& jArray);
    static std::string toString(const nlohmann::json& value);
};