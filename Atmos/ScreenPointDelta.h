#pragma once

#include "ScreenPoint.h"

namespace Atmos::Spatial
{
    class ScreenPointDelta
    {
    public:
        ScreenPoint from;
        ScreenPoint to;

        ScreenPointDelta() = default;
        ScreenPointDelta(ScreenPoint from, ScreenPoint to);

        [[nodiscard]] ScreenPoint Delta() const;
    };
}