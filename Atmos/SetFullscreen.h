#pragma once

#include <Arca/Command.h>

namespace Atmos::Render
{
    struct SetFullscreen
    {
        bool to;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Render::SetFullscreen>
    {
        static const ObjectType objectType = ObjectType::Command;
        static inline const TypeName typeName = "Atmos::Render::SetFullscreen";
    };
}