#pragma once

#include "ScreenPosition.h"

namespace Atmos
{
    class ScreenPositionDelta
    {
    public:
        const ScreenPosition from{};
        const ScreenPosition to{};
        const ScreenPosition delta{};

        ScreenPositionDelta() = default;
        ScreenPositionDelta(ScreenPosition from, ScreenPosition to);
    };
}