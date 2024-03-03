#pragma once

#include <Arca/Signal.h>

namespace Atmos::Render
{
    struct ReconstructGraphics
    {};
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Render::ReconstructGraphics>
    {
        static const ObjectType objectType = ObjectType::Signal;
        static const TypeName typeName;
    };
}