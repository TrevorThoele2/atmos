#pragma once

#include "InputManager.h"

namespace Atmos::Input
{
    class NullManager final : public Manager
    {
    public:
        NullManager(Logging::Logger& logger);

        [[nodiscard]] State ReadState() const override;
    };
}