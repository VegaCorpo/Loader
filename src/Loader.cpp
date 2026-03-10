#include "Loader.hpp"
#include <components/acceleration.hpp>
#include <components/mass.hpp>
#include <components/position.hpp>
#include <components/velocity.hpp>
#include <entt/entity/fwd.hpp>
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

loader::SimulationLoader::SimulationLoader(entt::registry& registry, const std::string& filename) : registry(registry)
{
    std::ifstream f(filename);
    if (!f.is_open())
        throw loader::SimulationLoader::SimulationLoaderError("Failed to open filestream",
                                                              common::LoaderStatus::FAILED_TO_OPEN_FILESTREAM);
    this->data = json::parse(f);
}

void loader::SimulationLoader::createEntities()
{
    if (this->data.contains("entities") && this->data["entities"].is_array()) {
        for (const json& it : this->data["entities"]) {
            auto entity = registry.create();
            loader::SimulationLoader::_createComponents(entity, it);
        }
    }
}

void loader::SimulationLoader::_createComponents(const entt::entity& entity, const json& entityData)
{
    if (entityData.contains("components") && entityData["components"].is_object()) {
        const json& componentsData = entityData["components"];
        this->_createMassCpn(entity, componentsData);
        this->_createAccelerationCpn(entity, componentsData);
        this->_createPositionCpn(entity, componentsData);
        this->_createVelocityCpn(entity, componentsData);
    }
}

void loader::SimulationLoader::_createMassCpn(const entt::entity& entity, const json& components)
{
    if (components.contains("Mass") && components["Mass"].is_object()) {
        const auto& massJson = components["Mass"];
        float mantissa = massJson.value("mantissa", 0.0f);
        int exponent = massJson.value("exponent", 0);

        this->registry.emplace<components::Mass>(entity, mantissa, exponent);
    }
}

void loader::SimulationLoader::_createAccelerationCpn(const entt::entity& entity, const json& components)
{
    if (components.contains("Acceleration") && components["Acceleration"].is_object()) {
        const auto& accelerationJson = components["Acceleration"];
        float x = accelerationJson.value("x", 0.0f);
        float y = accelerationJson.value("y", 0.0f);

        this->registry.emplace<components::Acceleration>(entity, x, y);
    }
}

void loader::SimulationLoader::_createPositionCpn(const entt::entity& entity, const nlohmann::json& components) 
{
    if (components.contains("Position") && components["Position"].is_object()) {
        const auto& posJson = components["Position"];
        float x = posJson.value("x", 0.0f);
        float y = posJson.value("y", 0.0f);
        float z = posJson.value("z", 0.0f);

        this->registry.emplace<::components::Position>(entity, x, y, z);
    }
}

void loader::SimulationLoader::_createVelocityCpn(const entt::entity& entity, const nlohmann::json& components) 
{
    if (components.contains("Velocity") && components["Velocity"].is_object()) {
        const auto& velJson = components["Velocity"];
        
        float x = velJson.value("x", 0.0f);
        float y = velJson.value("y", 0.0f);
        float z = velJson.value("z", 0.0f);

        this->registry.emplace<::components::Velocity>(entity, x, y, z);
    }
}
