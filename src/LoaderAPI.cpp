#include "LoaderAPI.hpp"
#include <iostream>
#include "Loader.hpp"
#include <entt/entity/registry.hpp>

extern "C" {
    common::LoaderStatus createScene(void *registry_ptr, const std::string& filename) noexcept
    {
        try {
            auto& registry = *static_cast<entt::registry*>(registry_ptr);
            loader::SimulationLoader loader(registry, filename);
            loader.createEntities();
        }
        catch (const loader::SimulationLoader::SimulationLoaderError& e) {
            std::cerr << e.what();
            return e.getType();
        }
        return common::LoaderStatus::SUCCESS;
    }
}
