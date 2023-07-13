#include "Shaders.h"

namespace Atmos::Render
{
    bool Shaders::operator==(const Shaders& arg) const
    {
        return IndexEqualsValue(vertex, arg.vertex) && IndexEqualsValue(fragment, arg.fragment);
    }

    bool Shaders::operator!=(const Shaders& arg) const
    {
        return !(*this == arg);
    }
}