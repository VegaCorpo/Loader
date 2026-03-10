#pragma once

#include <entt/entity/fwd.hpp>
#include <nlohmann/json.hpp>
#include <string>
#include <types/types.hpp>
#include <entt/entity/registry.hpp>

namespace loader {
    class SimulationLoader {
        public:
            class SimulationLoaderError : public std::exception {
                public:
                    explicit SimulationLoaderError(std::string msg, common::LoaderStatus type) :
                        _msg(std::move(msg)), _type(type)
                    {}

                    [[nodiscard]] const char* what() const noexcept override { return this->_msg.c_str(); };
                    [[nodiscard]] common::LoaderStatus getType() const noexcept { return this->_type; };

                private:
                    common::LoaderStatus _type;
                    std::string _msg;
            };

            explicit SimulationLoader(entt::registry& registry, const std::string& filename);

            void createEntities();

        private:
            void _createComponents(const entt::entity &entity, const nlohmann::json&);
            void _createMassCpn(const entt::entity &entity, const nlohmann::json&);
            void _createAccelerationCpn(const entt::entity& entity, const nlohmann::json& components);
            void _createPositionCpn(const entt::entity& entity, const nlohmann::json& components);
            void _createVelocityCpn(const entt::entity& entity, const nlohmann::json& components);

            nlohmann::json data;
            entt::registry& registry;
    };
} // namespace loader
