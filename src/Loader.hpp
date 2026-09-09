#pragma once

#include <entt/entity/fwd.hpp>
#include <entt/entity/registry.hpp>
#include <functional>
#include <nlohmann/json.hpp>
#include <string>
#include <types/types.hpp>
#include <unordered_map>
#include "interfaces/ILoaderEngine.hpp"

namespace loader {
    class SimulationLoader : public common::ILoaderEngine {
        public:
            SimulationLoader() = default;

            common::LoaderStatus createScene(void* registry_ptr, const std::string& filename) override;

            void createEntities(entt::registry& registry);

            void registerMapLoader();

            [[nodiscard]] common::LoaderStatus getLoaderStatus() const { return this->_status; };

        private:
            void _createComponents(entt::registry&, const entt::entity& entity, const nlohmann::json&);
            void _createMassCpn(entt::registry&, const entt::entity& entity, const nlohmann::json&);
            void _createAccelerationCpn(entt::registry&, const entt::entity& entity, const nlohmann::json& components);
            void _createPositionCpn(entt::registry&, const entt::entity& entity, const nlohmann::json& components);
            void _createVelocityCpn(entt::registry&, const entt::entity& entity, const nlohmann::json& components);
            void _createNameCpn(entt::registry&, const entt::entity& entity, const nlohmann::json& components);
            void _createRadiusCpn(entt::registry&, const entt::entity& entity, const nlohmann::json& components);
            void _createTextureCpn(entt::registry&, const entt::entity& entity, const nlohmann::json& components);

            std::unordered_map<std::string,
                std::function<void(entt::registry&, const entt::entity&, const nlohmann::json&)>> _mapLoader;
            common::LoaderStatus _status = common::LoaderStatus::SUCCESS;
            nlohmann::json _data;
    };
} // namespace loader
