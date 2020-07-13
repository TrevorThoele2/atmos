#pragma once

#include <Arca/Command.h>
#include "ScreenSize.h"

namespace Atmos::Render
{
    struct ReconstructGraphics
    {
        Spatial::ScreenSize screenSize;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Render::ReconstructGraphics>
    {
        static const ObjectType objectType = ObjectType::Command;
        static inline const TypeName typeName = "Atmos::Render::ReconstructGraphics";
    };
}