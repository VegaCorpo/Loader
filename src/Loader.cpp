#include "Loader.hpp"
#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

loader::SimulationLoader::SimulationLoader(const std::string& filename)
{
    std::ifstream f(filename);
    if (!f.is_open())
        throw loader::SimulationLoader::SimulationLoaderError("Failed to open filestream",
                                                              common::LoaderStatus::FAILED_TO_OPEN_FILESTREAM);
    this->data = json::parse(f);
}

void loader::SimulationLoader::createEntities(entt::registry& registry)
{
    for (const auto& it : this->data["entities"]) {
        auto entity = registry.create();
    }
}
