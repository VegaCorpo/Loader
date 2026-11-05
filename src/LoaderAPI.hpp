#pragma once

#include <memory>
#include <types/types.hpp>

#include "interfaces/ILoaderEngine.hpp"
#include "types/ModuleInfo.hpp"

extern "C" {
    std::unique_ptr<common::ILoaderEngine> get_engine();
}
