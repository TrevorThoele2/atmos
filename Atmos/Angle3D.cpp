#include "Angle3D.h"

namespace Atmos::Spatial
{
    bool Angle3D::operator==(const Angle3D& arg) const
    {
        return yaw == arg.yaw && pitch == arg.pitch;
    }

    bool Angle3D::operator!=(const Angle3D& arg) const
    {
        return !(*this == arg);
    }
}