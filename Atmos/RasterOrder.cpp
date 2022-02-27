#include "RasterOrder.h"

namespace Atmos::Render::Raster
{
    bool Order::operator==(const Order& arg) const
    {
        return space == arg.space && z == arg.z;
    }

    bool Order::operator!=(const Order& arg) const
    {
        return !(*this == arg);
    }

    bool Order::operator<(const Order& arg) const
    {
        return space != arg.space ? space < arg.space : z < arg.z;
    }
}
