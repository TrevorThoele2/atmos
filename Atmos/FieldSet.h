#pragma once

#include <Arca/Signal.h>
#include "Field.h"

namespace Atmos::World
{
    class FieldSet
    {
    public:
        Field& field;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::World::FieldSet>
    {
        static constexpr ObjectType objectType = ObjectType::Signal;
        static const inline TypeName typeName = "Atmos::World::FieldSet";
    };
}