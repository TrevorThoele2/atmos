#pragma once

#include <Arca/SignalTraits.h>
#include "Entity.h"

namespace Atmos::Entity
{
    struct MoveEntityDirection
    {
        Entity* entity = nullptr;
        Direction direction;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Entity::MoveEntityDirection>
    {
        static const ObjectType objectType = ObjectType::Signal;
        static const TypeName typeName;
    };
}