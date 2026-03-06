#pragma once

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

            explicit SimulationLoader(const std::string& filename);

            void createEntities(entt::registry& registry);

        private:
            nlohmann::json data;
    };
} // namespace loader
