#include "loader.hpp"

Loader::DataMap Loader::load(const std::string& filepath)
{
    std::ifstream file(filepath);
    if (!file.is_open()) return {};

    nlohmann::json j;
    file >> j;
    file.close();

    DataMap result;

    for (auto& [key, value] : j.items())
    {
        if (value.is_array())
            result[key] = composeDataVector(value);
        else if (value.is_object())
            result[key] = composeDataVector(nlohmann::json::array({ value }));
        else
            result[key] = {};
    }

    return result;
}

Loader::DataVector Loader::composeDataVector(const nlohmann::json& jArray)
{
    DataVector result;
    if (!jArray.is_array()) return result;

    for (auto& obj : jArray)
    {
        if (!obj.is_object()) continue;

        ObjectMap map;
        for (auto& [k, v] : obj.items())
        {
            if (v.is_array() && !v.empty() && v[0].is_object())
                map[k] = composeDataVector(v);
            else
                map[k] = toString(v);
        }

        result.push_back(map);
    }

    return result;
}

std::string Loader::toString(const nlohmann::json& value)
{
    if (value.is_string())  return value.get<std::string>();
    if (value.is_number())  return std::to_string(value.get<double>());
    if (value.is_boolean()) return value.get<bool>() ? "true" : "false";
    if (value.is_null())    return "null";
    return value.dump();
}