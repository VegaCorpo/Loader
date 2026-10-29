#include "LoaderAPI.hpp"
#include <memory>
#include "Loader.hpp"
#include "interfaces/ILoaderEngine.hpp"

extern "C" {
    std::unique_ptr<common::ILoaderEngine> get_engine()
    {
        return std::make_unique<loader::SimulationLoader>();
    }

    const common::ModuleInfo* get_module_info()
    {
        static constexpr common::ModuleInfo info{
            .name = ORBITAL_MODULE_NAME,
            .version = ORBITAL_MODULE_VERSION,
            .type = common::ModuleType::LOADER,
        };
        return &info;
    }
}
