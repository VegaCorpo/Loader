#pragma once

#include <entt/entity/registry.hpp>
#include <types/types.hpp>

#include "interfaces/ILoaderEngine.hpp"

extern "C" {
    std::unique_ptr<common::ILoaderEngine> get_engine();
}
