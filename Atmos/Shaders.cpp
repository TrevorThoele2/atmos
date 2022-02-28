#include "Shaders.h"

namespace Atmos::Render
{
    bool Shaders::operator==(const Shaders& arg) const
    {
        return vertex == arg.vertex && fragment == arg.fragment;
    }

    bool Shaders::operator!=(const Shaders& arg) const
    {
        return !(*this == arg);
    }
}