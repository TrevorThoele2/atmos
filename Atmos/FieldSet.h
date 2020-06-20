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
        static const ObjectType objectType = ObjectType::Signal;
        static inline const TypeName typeName = "Atmos::World::FieldSet";
    };
}