#pragma once

#include <Arca/Command.h>

namespace Atmos::Render
{
    struct ChangeVerticalSync
    {
        bool to;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Render::ChangeVerticalSync>
    {
        static constexpr ObjectType objectType = ObjectType::Command;
        static const inline TypeName typeName = "Atmos::Render::ChangeVerticalSync";
    };
}