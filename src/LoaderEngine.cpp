#include <iostream>
#include <memory>
#include "LoaderEngine.hpp"

extern "C++" {
    std::unique_ptr<loader::LoaderEngine> get_inputs_engine()
    {
        std::cout << "Hello from loader engine" << std::endl;
        return std::make_unique<loader::LoaderEngine>();
    }
}
