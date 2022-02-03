#pragma once

#include <optional>
#include "Point2D.h"
#include "Angle2D.h"

namespace Atmos::Spatial
{
    struct Line2D
    {
        Point2D from;
        Point2D to;

        bool operator==(const Line2D& arg) const;
        bool operator!=(const Line2D& arg) const;
    };
}