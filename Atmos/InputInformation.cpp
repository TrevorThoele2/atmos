#include "InputInformation.h"

namespace Atmos::Input
{
    Information& Information::operator=(Information&& arg) noexcept
    {
        mousePosition = arg.mousePosition;
        return *this;
    }

    bool Information::operator==(const Information& arg) const
    {
        return mousePosition == arg.mousePosition;
    }

    bool Information::operator!=(const Information& arg) const
    {
        return !(*this == arg);
    }
}