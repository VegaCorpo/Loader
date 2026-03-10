#include "LoaderAPI.hpp"
#include <entt/entity/registry.hpp>
#include <types/types.hpp>
#include "Loader.hpp"

extern "C" {
    common::LoaderStatus createScene(void* registry_ptr, const std::string& filename) noexcept
    {
        auto& registry = *static_cast<entt::registry*>(registry_ptr);
        loader::SimulationLoader loader(registry, filename);
        if (loader.getLoaderStatus() == common::LoaderStatus::FAILED_TO_OPEN_FILESTREAM)
            return common::LoaderStatus::FAILED_TO_OPEN_FILESTREAM;
        loader.registerMapLoader();
        loader.createEntities();
        return common::LoaderStatus::SUCCESS;
    }
}
