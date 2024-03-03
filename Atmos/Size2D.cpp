#include "Size2D.h"

namespace Atmos::Spatial
{
    bool Size2D::operator==(const Size2D& arg) const
    {
        return width == arg.width && height == arg.height;
    }

    bool Size2D::operator!=(const Size2D& arg) const
    {
        return !(*this == arg);
    }
}