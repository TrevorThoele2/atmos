#include "Line3D.h"

namespace Atmos::Spatial
{
    bool Line3D::operator==(const Line3D& arg) const
    {
        return first == arg.first && second == arg.second;
    }

    bool Line3D::operator!=(const Line3D& arg) const
    {
        return !(*this == arg);
    }
}