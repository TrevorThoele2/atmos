#include "ScreenSize.h"

namespace Atmos::Spatial
{
    ScreenSize::ScreenSize(Dimension width, Dimension height) : width(width), height(height)
    {}

    bool ScreenSize::operator==(const ScreenSize& arg) const
    {
        return width == arg.width && height == arg.height;
    }

    bool ScreenSize::operator!=(const ScreenSize& arg) const
    {
        return !(*this == arg);
    }
}