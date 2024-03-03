#include "Scalers2D.h"

namespace Atmos::Spatial
{
    bool Scalers2D::operator==(const Scalers2D& arg) const
    {
        return x == arg.x && y == arg.y;
    }

    bool Scalers2D::operator!=(const Scalers2D& arg) const
    {
        return !(*this == arg);
    }
}