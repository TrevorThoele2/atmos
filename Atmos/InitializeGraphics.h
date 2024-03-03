#pragma once

#include <Arca/Command.h>

namespace Atmos::Render
{
    struct InitializeGraphics
    {
        void* window;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Render::InitializeGraphics>
    {
        static const ObjectType objectType = ObjectType::Command;
        static inline const TypeName typeName = "Atmos::Render::InitializeGraphics";
    };
}