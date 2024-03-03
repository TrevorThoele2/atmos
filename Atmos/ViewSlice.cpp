#include "ViewSlice.h"

namespace Atmos::Render
{
    ViewSlice::ViewSlice(const Spatial::AxisAlignedBox2D& box) : box(box)
    {}

    bool ViewSlice::operator==(const ViewSlice& arg) const
    {
        return box == arg.box;
    }

    bool ViewSlice::operator!=(const ViewSlice& arg) const
    {
        return !(*this == arg);
    }
}