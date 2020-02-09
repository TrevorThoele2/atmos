#pragma once

#include "ScreenPosition.h"

namespace Atmos
{
    class ScreenPositionDelta
    {
    public:
        ScreenPosition from;
        ScreenPosition to;

        ScreenPositionDelta() = default;
        ScreenPositionDelta(ScreenPosition from, ScreenPosition to);

        [[nodiscard]] ScreenPosition Delta() const;
    };
}