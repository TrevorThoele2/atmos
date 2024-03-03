#pragma once

#include <Arca/Command.h>
#include "FieldID.h"

namespace Atmos::World
{
    struct RequestField
    {
        FieldID id;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::World::RequestField>
    {
        static const ObjectType objectType = ObjectType::Command;
        static TypeName TypeName() { return "Atmos::World::RequestField"; }
    };
}