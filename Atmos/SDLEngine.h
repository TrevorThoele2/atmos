#pragma once

#include "Engine.h"

namespace Atmos
{
    class SDLEngine final : public Engine
    {
    public:
        SDLEngine(Logging::Logger& logger);
    protected:
        InitializationProperties CreateInitializationProperties(Logging::Logger& logger) override;
    };
}