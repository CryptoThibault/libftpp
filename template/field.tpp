template<typename T>
std::vector<T> Field::asVectorOf() const {
    std::vector<T> result;
    for (const auto& f : std::get<DataVector>(_value)) {
        result.push_back(f);
    }
    return result;
}

template<typename T>
std::map<std::string, T> Field::asMapOf() const {
    std::map<std::string, T> result;
    for (const auto& [k,v] : std::get<DataMap>(_value)) {
        result[k] = v;
    }
    return result;
}