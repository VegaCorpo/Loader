#pragma once

#include <entt/entity/fwd.hpp>
#include <entt/entity/registry.hpp>
#include <functional>
#include <nlohmann/json.hpp>
#include <string>
#include <types/types.hpp>
#include <unordered_map>

namespace loader {
    class SimulationLoader {
        public:
            explicit SimulationLoader(entt::registry& registry, const std::string& filename);

            void createEntities();

            void registerMapLoader();

            [[nodiscard]] common::LoaderStatus getLoaderStatus() const { return this->_status; };

        private:
            void _createComponents(const entt::entity& entity, const nlohmann::json&);
            void _createMassCpn(const entt::entity& entity, const nlohmann::json&);
            void _createAccelerationCpn(const entt::entity& entity, const nlohmann::json& components);
            void _createPositionCpn(const entt::entity& entity, const nlohmann::json& components);
            void _createVelocityCpn(const entt::entity& entity, const nlohmann::json& components);
            void _createNameCpn(const entt::entity& entity, const nlohmann::json& components);
            void _createRadiusCpn(const entt::entity& entity, const nlohmann::json& components);
            void _createTextureIdCpn(const entt::entity& entity, const nlohmann::json& components);

            std::unordered_map<std::string, std::function<void(const entt::entity&, const nlohmann::json&)>> _mapLoader;
            common::LoaderStatus _status;
            nlohmann::json _data;
            entt::registry& _registry;
    };
} // namespace loader
