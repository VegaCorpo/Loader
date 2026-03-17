#include "Loader.hpp"
#include <components/acceleration.hpp>
#include <components/mass.hpp>
#include <components/position.hpp>
#include <components/velocity.hpp>
#include <entt/entity/fwd.hpp>
#include <fstream>
#include <nlohmann/json.hpp>
#include <types/types.hpp>

using json = nlohmann::json;

loader::SimulationLoader::SimulationLoader(entt::registry& registry, const std::string& filename) :
    _registry(registry), _status(common::LoaderStatus::SUCCESS)
{
    std::ifstream file(filename);
    if (!file.is_open()) {
        this->_status = common::LoaderStatus::FAILED_TO_OPEN_FILESTREAM;
        return;
    }
    this->_data = json::parse(file);
}

void loader::SimulationLoader::registerMapLoader()
{
    this->_mapLoader.insert({"Velocity", [this](const entt::entity& entity, const json& componentJson)
                             { this->_createVelocityCpn(entity, componentJson); }});
    this->_mapLoader.insert({"Mass", [this](const entt::entity& entity, const json& componentJson)
                             { this->_createMassCpn(entity, componentJson); }});
    this->_mapLoader.insert({"Position", [this](const entt::entity& entity, const json& componentJson)
                             { this->_createPositionCpn(entity, componentJson); }});
    this->_mapLoader.insert({"Acceleration", [this](const entt::entity& entity, const json& componentJson)
                             { this->_createAccelerationCpn(entity, componentJson); }});
}

void loader::SimulationLoader::createEntities()
{
    if (this->_data.contains("entities") && this->_data["entities"].is_array()) {
        for (const json& it : this->_data["entities"]) {
            auto entity = this->_registry.create();
            loader::SimulationLoader::_createComponents(entity, it);
        }
    }
}

void loader::SimulationLoader::_createComponents(const entt::entity& entity, const json& entityData)
{
    if (entityData.contains("components") && entityData["components"].is_object()) {
        const json& componentsData = entityData["components"];
        for (const auto& item : componentsData.items()) {
            if (this->_mapLoader.find(item.key()) != this->_mapLoader.end())
                this->_mapLoader.at(item.key())(entity, componentsData);
        }
    }
}

void loader::SimulationLoader::_createMassCpn(const entt::entity& entity, const json& components)
{
    const auto& massJson = components["Mass"];
    float mantissa = massJson.value("mantissa", 0.0f);
    int exponent = massJson.value("exponent", 0);

    this->_registry.emplace_or_replace<common::components::Mass>(entity, mantissa, exponent);
}

void loader::SimulationLoader::_createAccelerationCpn(const entt::entity& entity, const json& components)
{
    const auto& accelerationJson = components["Acceleration"];
    float x = accelerationJson.value("x", 0.0f);
    float y = accelerationJson.value("y", 0.0f);

    this->_registry.emplace_or_replace<common::components::Acceleration>(entity, x, y);
}

void loader::SimulationLoader::_createPositionCpn(const entt::entity& entity, const nlohmann::json& components)
{
    const auto& posJson = components["Position"];
    float x = posJson.value("x", 0.0f);
    float y = posJson.value("y", 0.0f);
    float z = posJson.value("z", 0.0f);

    this->_registry.emplace_or_replace<common::components::Position>(entity, x, y, z);
}

void loader::SimulationLoader::_createVelocityCpn(const entt::entity& entity, const nlohmann::json& components)
{
    const auto& velJson = components["Velocity"];

    float x = velJson.value("x", 0.0f);
    float y = velJson.value("y", 0.0f);
    float z = velJson.value("z", 0.0f);

    this->_registry.emplace_or_replace<common::components::Velocity>(entity, x, y, z);
}
