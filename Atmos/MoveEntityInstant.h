#pragma once

#include <Arca/Signal.h>
#include "Entity.h"

namespace Atmos::Entity
{
    struct MoveEntityInstant
    {
        Arca::Index<Entity> entity;
        Grid::Position toPosition;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Entity::MoveEntityInstant>
    {
        static const ObjectType objectType = ObjectType::Command;
        static inline const TypeName typeName = "Atmos::Entity::MoveEntityInstant";
    };
}