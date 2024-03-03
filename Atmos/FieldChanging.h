#pragma once

#include <Arca/Signal.h>

namespace Atmos::World
{
    struct FieldChanging
    {};
}

namespace Arca
{
    template<>
    struct Traits<Atmos::World::FieldChanging>
    {
        static constexpr ObjectType objectType = ObjectType::Signal;
        static const inline TypeName typeName = "Atmos::World::FieldChanging";
    };
}