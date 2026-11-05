#include "Loader.hpp"
#include <array>
#include <components/acceleration.hpp>
#include <components/angularVelocity.hpp>
#include <components/mass.hpp>
#include <components/name.hpp>
#include <components/orientation.hpp>
#include <components/position.hpp>
#include <components/radius.hpp>
#include <components/texture.hpp>
#include <components/velocity.hpp>
#include <fstream>
#include <nlohmann/json.hpp>
#include <numbers>
#include <types/types.hpp>
#include <utils/assets.hpp>
#include <utils/spin.hpp>

using json = nlohmann::json;

namespace {
    constexpr double DEGREES_TO_RADIANS = std::numbers::pi / 180.0;
    constexpr std::array<const char*, 7> REQUIRED_COMPONENTS = {
        "Name", "Position", "Velocity", "Acceleration", "Mass", "Radius", "Texture",
    };
} // namespace

std::expected<common::SceneDescription, common::LoaderStatus>
    loader::SimulationLoader::createScene(const std::string& filename)
{
    std::ifstream file(filename);
    if (!file.is_open()) {
        this->_status = common::LoaderStatus::FAILED_TO_OPEN_FILESTREAM;
        return std::unexpected(common::LoaderStatus::FAILED_TO_OPEN_FILESTREAM);
    }
    this->_data = json::parse(file);

    this->registerMapLoader();

    common::SceneDescription scene;

    this->_status = this->createBodies(scene);
    if (this->_status != common::LoaderStatus::SUCCESS)
        return std::unexpected(this->_status);
    return scene;
}

void loader::SimulationLoader::registerMapLoader()
{
    this->_mapLoader.insert({"Velocity",
                             [this](common::BodyDescription& body, const json& componentJson)
                             { this->_createVelocityCpn(body, componentJson); }});
    this->_mapLoader.insert({"Mass",
                             [this](common::BodyDescription& body, const json& componentJson)
                             { this->_createMassCpn(body, componentJson); }});
    this->_mapLoader.insert({"Position",
                             [this](common::BodyDescription& body, const json& componentJson)
                             { this->_createPositionCpn(body, componentJson); }});
    this->_mapLoader.insert({"Acceleration",
                             [this](common::BodyDescription& body, const json& componentJson)
                             { this->_createAccelerationCpn(body, componentJson); }});
    this->_mapLoader.insert({"Name",
                             [this](common::BodyDescription& body, const json& componentJson)
                             { this->_createNameCpn(body, componentJson); }});
    this->_mapLoader.insert({"Radius",
                             [this](common::BodyDescription& body, const json& componentJson)
                             { this->_createRadiusCpn(body, componentJson); }});
    this->_mapLoader.insert({"Texture",
                             [this](common::BodyDescription& body, const json& componentJson)
                             { this->_createTextureCpn(body, componentJson); }});
    this->_mapLoader.insert({"Rotation",
                             [this](common::BodyDescription& body, const json& componentJson)
                             { this->_createRotationCpn(body, componentJson); }});
}

common::LoaderStatus loader::SimulationLoader::createBodies(common::SceneDescription& scene)
{
    if (!this->_data.contains("entities") || !this->_data["entities"].is_array())
        return common::LoaderStatus::SUCCESS;

    for (const json& it : this->_data["entities"]) {
        common::BodyDescription body{};

        body.id = scene.bodies.size();
        if (!this->_createComponents(body, it))
            return common::LoaderStatus::WRONG_SIMULATION_FORMAT;
        scene.bodies.push_back(body);
    }
    return common::LoaderStatus::SUCCESS;
}

bool loader::SimulationLoader::_createComponents(common::BodyDescription& body, const json& entityData)
{
    if (!entityData.contains("components") || !entityData["components"].is_object())
        return false;

    const json& componentsData = entityData["components"];

    for (const char* required : REQUIRED_COMPONENTS) {
        if (!componentsData.contains(required))
            return false;
    }
    for (const auto& item : componentsData.items()) {
        if (this->_mapLoader.find(item.key()) != this->_mapLoader.end())
            this->_mapLoader.at(item.key())(body, componentsData);
    }
    return true;
}

void loader::SimulationLoader::_createMassCpn(common::BodyDescription& body, const json& components)
{
    const auto& massJson = components["Mass"];
    float mantissa = massJson.value("mantissa", 0.0f);
    int exponent = massJson.value("exponent", 0);

    body.mass = {mantissa, exponent};
}

void loader::SimulationLoader::_createAccelerationCpn(common::BodyDescription& body, const json& components)
{
    const auto& accelerationJson = components["Acceleration"];
    float x = accelerationJson.value("x", 0.0f);
    float y = accelerationJson.value("y", 0.0f);
    float z = accelerationJson.value("z", 0.0f);

    body.acceleration = {x, y, z};
}

void loader::SimulationLoader::_createPositionCpn(common::BodyDescription& body, const json& components)
{
    const auto& posJson = components["Position"];
    float x = posJson.value("x", 0.0f);
    float y = posJson.value("y", 0.0f);
    float z = posJson.value("z", 0.0f);

    body.position = {x, y, z};
}

void loader::SimulationLoader::_createVelocityCpn(common::BodyDescription& body, const json& components)
{
    const auto& velJson = components["Velocity"];

    float x = velJson.value("x", 0.0f);
    float y = velJson.value("y", 0.0f);
    float z = velJson.value("z", 0.0f);

    body.velocity = {x, y, z};
}

void loader::SimulationLoader::_createNameCpn(common::BodyDescription& body, const json& components)
{
    const auto& nameJson = components["Name"];
    std::string tmp = nameJson.value("value", "");

    common::components::Name name{};
    std::strncpy(name.value, tmp.c_str(), common::components::MAX_NAME_LENGTH - 1);

    body.name = name;
}

void loader::SimulationLoader::_createRadiusCpn(common::BodyDescription& body, const json& components)
{
    const auto& radiusJson = components["Radius"];

    float radius = radiusJson.value("value", 0.0f);

    body.radius = {radius};
}

void loader::SimulationLoader::_createTextureCpn(common::BodyDescription& body, const json& components)
{
    const auto& textureJson = components["Texture"];
    std::string tmp = textureJson.value("path", common::DEFAULT_TEXTURE_PATH);

    common::components::Texture tex{};
    std::strncpy(tex.path, tmp.c_str(), common::components::MAX_TEXTURE_PATH_LENGTH - 1);

    body.texture = tex;
}

void loader::SimulationLoader::_createRotationCpn(common::BodyDescription& body, const json& components)
{
    const auto spin = loader::SimulationLoader::_readSpinParameters(components["Rotation"]);

    body.orientation = common::rotation::initialOrientation(spin);
    body.angularVelocity = common::rotation::angularVelocity(spin);
}

common::rotation::SpinParameters loader::SimulationLoader::_readSpinParameters(const nlohmann::json& rotationJson)
{
    return {
        rotationJson.value("period", 0.0),
        rotationJson.value("obliquity", 0.0) * DEGREES_TO_RADIANS,
        rotationJson.value("axisAzimuth", 0.0) * DEGREES_TO_RADIANS,
        rotationJson.value("initialAngle", 0.0) * DEGREES_TO_RADIANS,
    };
}
