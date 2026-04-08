#pragma once
#include "singleton.hpp"
#include "field.hpp"
#include <vector>
#include <functional>
#include <stdexcept>

class Factory : public Singleton<Factory>
{
public:
    template <typename T>
    T create(const std::string& name, const DataMap& map);
    
    template <typename T>
    std::vector<T> createAll(const std::string& name, const DataVector& vec);

    template <typename T>
    void registerCreator(const std::string& name, const std::function<T(const DataMap&)>& creator);

private:
    std::map<std::string, std::function<void*(const DataMap&)>> _creators;
};

#include "factory.tpp"