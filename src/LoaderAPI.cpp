#include "LoaderAPI.hpp"
#include <iostream>
#include "Loader.hpp"

extern "C" {
    common::LoaderStatus createScene(entt::registry& registry, const std::string& filename) noexcept
    {
        try {
            std::cout << "Enter in loader\n";
            loader::SimulationLoader loader(filename);
            loader.createEntities(registry);
        }
        catch (const loader::SimulationLoader::SimulationLoaderError& e) {
            std::cerr << e.what();
            return e.getType();
        }
        return common::LoaderStatus::SUCCESS;
    }
}
