#pragma once

#include <Arca/Signal.h>
#include "Field.h"

namespace Atmos::World
{
    class FieldUnset
    {
    public:
        Field& field;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::World::FieldUnset>
    {
        static const ObjectType objectType = ObjectType::Signal;
        static inline const TypeName typeName = "FieldUnset";
    };
}