#pragma once

#include <stdexcept>

namespace Atmos
{
    class EngineNotSetup : public std::runtime_error
    {
    public:
        EngineNotSetup();
    };
}