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
        static constexpr ObjectType objectType = ObjectType::Command;
        static const inline TypeName typeName = "Atmos::World::RequestField";
    };
}