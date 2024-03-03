#include "Size3D.h"

namespace Atmos::Spatial
{
    bool Size3D::operator==(const Size3D& arg) const
    {
        return width == arg.width && height == arg.height && depth == arg.depth;
    }

    bool Size3D::operator!=(const Size3D& arg) const
    {
        return !(*this == arg);
    }
}