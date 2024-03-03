#include "InputInformation.h"

namespace Atmos::Input
{
    Information& Information::operator=(Information&& arg) noexcept
    {
        mousePosition = arg.mousePosition;
        return *this;
    }
}