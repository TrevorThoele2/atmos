#include "Point3D.h"

namespace Atmos::Spatial
{
    bool Point3D::operator==(const Point3D& arg) const
    {
        return x == arg.x && y == arg.y && z == arg.z;
    }

    bool Point3D::operator!=(const Point3D& arg) const
    {
        return !(*this == arg);
    }
}