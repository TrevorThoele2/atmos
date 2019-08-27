#pragma once

#include <Arca/SignalTraits.h>
#include "Entity.h"

namespace Atmos::Entity
{
    struct MoveEntityInstant
    {
        Entity* entity = nullptr;
        Grid::Position toPosition;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Entity::MoveEntityInstant>
    {
        static const ObjectType objectType = ObjectType::Signal;
        static const TypeName typeName;
    };
}