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
        static const ObjectType objectType = ObjectType::Signal;
        static TypeName TypeName() { return "Atmos::World::FieldChanging"; }
    };
}