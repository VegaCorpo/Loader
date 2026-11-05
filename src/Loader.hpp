#pragma once

#include <expected>
#include <functional>
#include <nlohmann/json.hpp>
#include <string>
#include <types/Scene.hpp>
#include <types/types.hpp>
#include <unordered_map>
#include <utils/spin.hpp>
#include "interfaces/ILoaderEngine.hpp"

namespace loader {
    class SimulationLoader : public common::ILoaderEngine {
        public:
            SimulationLoader() = default;

            [[nodiscard]] std::expected<common::SceneDescription, common::LoaderStatus>
                createScene(const std::string& filename) override;

            [[nodiscard]] common::LoaderStatus createBodies(common::SceneDescription& scene);

            void registerMapLoader();

            [[nodiscard]] common::LoaderStatus getLoaderStatus() const { return this->_status; };

        private:
            [[nodiscard]] bool _createComponents(common::BodyDescription& body, const nlohmann::json& entityData);
            void _createMassCpn(common::BodyDescription& body, const nlohmann::json& components);
            void _createAccelerationCpn(common::BodyDescription& body, const nlohmann::json& components);
            void _createPositionCpn(common::BodyDescription& body, const nlohmann::json& components);
            void _createVelocityCpn(common::BodyDescription& body, const nlohmann::json& components);
            void _createNameCpn(common::BodyDescription& body, const nlohmann::json& components);
            void _createRadiusCpn(common::BodyDescription& body, const nlohmann::json& components);
            void _createTextureCpn(common::BodyDescription& body, const nlohmann::json& components);
            void _createRotationCpn(common::BodyDescription& body, const nlohmann::json& components);
            [[nodiscard]] static common::rotation::SpinParameters _readSpinParameters(const nlohmann::json& rotationJson);

            std::unordered_map<std::string, std::function<void(common::BodyDescription&, const nlohmann::json&)>>
                _mapLoader;
            common::LoaderStatus _status = common::LoaderStatus::SUCCESS;
            nlohmann::json _data;
    };
} // namespace loader
