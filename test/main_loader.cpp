#include "loader.hpp"
#include <iostream>

int main()
{
    // Charger le fichier JSON
    auto data = Loader::load("data.json");

    // Vérifier s'il y a des personnes
    if (data.find("People") != data.end()) {
        auto& people = data["People"];
        std::cout << "Loaded " << people.size() << " people\n\n";

        for (size_t i = 0; i < people.size(); ++i) {
            auto& person = people[i];
            std::cout << "Person " << i+1 << ":\n";

            for (auto& [k, v] : person) {
                std::cout << "  " << k << ": ";
                if (std::holds_alternative<std::string>(v)) {
                    std::cout << std::get<std::string>(v);
                }
                else if (std::holds_alternative<Loader::DataVector>(v)) {
                    std::cout << "[DataVector of size " 
                              << std::get<Loader::DataVector>(v).size() << "]";
                }
                std::cout << "\n";
            }
            std::cout << "\n";
        }
    } else {
        std::cout << "No People found in the JSON.\n";
    }

    return 0;
}