
#include "ScreenDimensions.h"

namespace Atmos
{
    ScreenDimensions::ScreenDimensions() : width(0), height(0)
    {}

    ScreenDimensions::ScreenDimensions(Dimension width, Dimension height) : width(width), height(height)
    {}

    ScreenDimensions::ScreenDimensions(const ScreenDimensions& arg) : width(arg.width), height(arg.height)
    {}

    bool ScreenDimensions::operator==(const ScreenDimensions& arg) const
    {
        return width == arg.width && height == arg.height;
    }

    bool ScreenDimensions::operator!=(const ScreenDimensions& arg) const
    {
        return !(*this == arg);
    }
}