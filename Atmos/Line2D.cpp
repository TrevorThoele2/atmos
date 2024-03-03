#include "Line2D.h"

namespace Atmos::Spatial
{
    bool Line2D::operator==(const Line2D& arg) const
    {
        return from == arg.from && to == arg.to;
    }

    bool Line2D::operator!=(const Line2D& arg) const
    {
        return !(*this == arg);
    }
}