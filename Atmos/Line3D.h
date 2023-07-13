#pragma once

#include "Point3D.h"

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