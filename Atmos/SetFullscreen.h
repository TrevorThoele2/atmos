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
        static constexpr ObjectType objectType = ObjectType::Command;
        static const inline TypeName typeName = "Atmos::Render::SetFullscreen";
    };
}