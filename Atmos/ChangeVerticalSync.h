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
        static const ObjectType objectType = ObjectType::Command;
        static inline const TypeName typeName = "ChangeVerticalSync";
    };
}