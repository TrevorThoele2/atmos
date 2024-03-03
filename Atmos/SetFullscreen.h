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
        static TypeName TypeName() { return "Atmos::Render::SetFullscreen"; }
    };
}