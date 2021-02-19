#include "GridSize.h"

namespace Atmos::Spatial::Grid
{
    bool Size::operator==(const Size& arg) const
    {
        return width == arg.width && height == arg.height;
    }

    bool Size::operator!=(const Size& arg) const
    {
        return !(*this == arg);
    }
}