#pragma once

#include <Arca/Command.h>
#include "Size2D.h"

namespace Atmos::Render
{
    struct ReconstructGraphics
    {
        Spatial::Size2D screenSize;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Render::ReconstructGraphics>
    {
        static const ObjectType objectType = ObjectType::Command;
        static TypeName TypeName() { return "Atmos::Render::ReconstructGraphics"; }
    };
}