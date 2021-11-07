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
        static constexpr ObjectType objectType = ObjectType::Command;
        static const inline TypeName typeName = "Atmos::Render::ReconstructGraphics";
    };
}