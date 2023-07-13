#pragma once

#include "Engine.h"

namespace Atmos
{
    class SDLEngine final : public Engine
    {
    public:
        explicit SDLEngine(Logging::Logger& logger);
    private:
        [[nodiscard]] static InitializationProperties CreateInitializationProperties(Logging::Logger& logger);
    };
}