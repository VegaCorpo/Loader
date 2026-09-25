#include "Loader.hpp"
#include <components/acceleration.hpp>
#include <components/mass.hpp>
#include <components/name.hpp>
#include <components/position.hpp>
#include <components/radius.hpp>
#include <components/texture.hpp>
#include <components/velocity.hpp>
#include <entt/entity/fwd.hpp>
#include <fstream>
#include <nlohmann/json.hpp>
#include <types/types.hpp>
#include <utils/assets.hpp>

using json = nlohmann::json;

common::LoaderStatus loader::SimulationLoader::createScene(void* registry_ptr, const std::string& filename)
{
    auto& registry = *static_cast<entt::registry*>(registry_ptr);

    std::ifstream file(filename);
    if (!file.is_open()) {
        this->_status = common::LoaderStatus::FAILED_TO_OPEN_FILESTREAM;
        return common::LoaderStatus::FAILED_TO_OPEN_FILESTREAM;
    }
    this->_data = json::parse(file);

    this->registerMapLoader();
    this->createEntities(registry);
    return common::LoaderStatus::SUCCESS;
}

void loader::SimulationLoader::registerMapLoader()
{
    this->_mapLoader.insert({"Velocity", [this](entt::registry& registry, const entt::entity& entity,
                                                const json& componentJson)
                             { this->_createVelocityCpn(registry, entity, componentJson); }});
    this->_mapLoader.insert({"Mass", [this](entt::registry& registry, const entt::entity& entity,
                                            const json& componentJson)
                             { this->_createMassCpn(registry, entity, componentJson); }});
    this->_mapLoader.insert({"Position", [this](entt::registry& registry, const entt::entity& entity,
                                                const json& componentJson)
                             { this->_createPositionCpn(registry, entity, componentJson); }});
    this->_mapLoader.insert({"Acceleration", [this](entt::registry& registry, const entt::entity& entity,
                                                    const json& componentJson)
                             { this->_createAccelerationCpn(registry, entity, componentJson); }});
    this->_mapLoader.insert({"Name", [this](entt::registry& registry, const entt::entity& entity,
                                            const json& componentJson)
                             { this->_createNameCpn(registry, entity, componentJson); }});
    this->_mapLoader.insert({"Radius", [this](entt::registry& registry, const entt::entity& entity,
                                              const json& componentJson)
                             { this->_createRadiusCpn(registry, entity, componentJson); }});
    this->_mapLoader.insert({"Texture", [this](entt::registry& registry, const entt::entity& entity,
                                               const json& componentJson)
                             { this->_createTextureCpn(registry, entity, componentJson); }});
}

void loader::SimulationLoader::createEntities(entt::registry& registry)
{
    if (this->_data.contains("entities") && this->_data["entities"].is_array()) {
        for (const json& it : this->_data["entities"]) {
            auto entity = registry.create();
            this->_createComponents(registry, entity, it);
        }
    }
}

void loader::SimulationLoader::_createComponents(entt::registry& registry, const entt::entity& entity,
                                                 const json& entityData)
{
    if (entityData.contains("components") && entityData["components"].is_object()) {
        const json& componentsData = entityData["components"];
        for (const auto& item : componentsData.items()) {
            if (this->_mapLoader.find(item.key()) != this->_mapLoader.end())
                this->_mapLoader.at(item.key())(registry, entity, componentsData);
        }
    }
}

void loader::SimulationLoader::_createMassCpn(entt::registry& registry, const entt::entity& entity,
                                              const json& components)
{
    const auto& massJson = components["Mass"];
    float mantissa = massJson.value("mantissa", 0.0f);
    int exponent = massJson.value("exponent", 0);

    registry.emplace_or_replace<common::components::Mass>(entity, mantissa, exponent);
}

void loader::SimulationLoader::_createAccelerationCpn(entt::registry& registry, const entt::entity& entity,
                                                      const json& components)
{
    const auto& accelerationJson = components["Acceleration"];
    float x = accelerationJson.value("x", 0.0f);
    float y = accelerationJson.value("y", 0.0f);

    registry.emplace_or_replace<common::components::Acceleration>(entity, x, y);
}

void loader::SimulationLoader::_createPositionCpn(entt::registry& registry, const entt::entity& entity,
                                                  const nlohmann::json& components)
{
    const auto& posJson = components["Position"];
    float x = posJson.value("x", 0.0f);
    float y = posJson.value("y", 0.0f);
    float z = posJson.value("z", 0.0f);

    registry.emplace_or_replace<common::components::Position>(entity, x, y, z);
}

void loader::SimulationLoader::_createVelocityCpn(entt::registry& registry, const entt::entity& entity,
                                                  const nlohmann::json& components)
{
    const auto& velJson = components["Velocity"];

    float x = velJson.value("x", 0.0f);
    float y = velJson.value("y", 0.0f);
    float z = velJson.value("z", 0.0f);

    registry.emplace_or_replace<common::components::Velocity>(entity, x, y, z);
}

void loader::SimulationLoader::_createNameCpn(entt::registry& registry, const entt::entity& entity,
                                              const nlohmann::json& components)
{
    const auto& nameJson = components["Name"];
    std::string tmp = nameJson.value("value", "");

    common::components::Name name{};
    std::strncpy(name.value, tmp.c_str(), common::components::MAX_NAME_LENGTH - 1);

    registry.emplace_or_replace<common::components::Name>(entity, name);
}

void loader::SimulationLoader::_createRadiusCpn(entt::registry& registry, const entt::entity& entity,
                                                const nlohmann::json& components)
{
    const auto& radiusJson = components["Radius"];

    float radius = radiusJson.value("value", 0.0f);

    registry.emplace_or_replace<common::components::Radius>(entity, radius);
}

void loader::SimulationLoader::_createTextureCpn(entt::registry& registry, const entt::entity& entity,
                                                 const nlohmann::json& components)
{
    const auto& textureJson = components["Texture"];
    std::string tmp = textureJson.value("path", common::DEFAULT_TEXTURE_PATH);

    common::components::Texture tex{};
    std::strncpy(tex.path, tmp.c_str(), common::components::MAX_TEXTURE_PATH_LENGTH - 1);

    registry.emplace_or_replace<common::components::Texture>(entity, tex);
}
