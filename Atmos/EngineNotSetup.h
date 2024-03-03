#pragma once

#include <stdexcept>

namespace Atmos
{
    class EngineNotSetup final : public std::runtime_error
    {
    public:
        EngineNotSetup();
    };
}