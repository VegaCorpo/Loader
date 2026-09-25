#include "LoaderAPI.hpp"
#include <memory>
#include "Loader.hpp"
#include "interfaces/ILoaderEngine.hpp"

extern "C" {
    std::unique_ptr<common::ILoaderEngine> get_engine()
    {
        return std::make_unique<loader::SimulationLoader>();
    }
}
