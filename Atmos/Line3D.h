#pragma once

#include <optional>

#include "Point3D.h"
#include "Angle2D.h"
#include "Vector3D.h"

namespace Atmos::Spatial
{
    struct Line3D
    {
        Point3D first;
        Point3D second;

        bool operator==(const Line3D& arg) const;
        bool operator!=(const Line3D& arg) const;
    };
}