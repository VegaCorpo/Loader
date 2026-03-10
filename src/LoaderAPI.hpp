#pragma once

#include <entt/entity/registry.hpp>
#include <types/types.hpp>

#include <string>

extern "C" {
    common::LoaderStatus createScene(void *registry ,const std::string& filename) noexcept;
}
