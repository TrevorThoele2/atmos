#pragma once

#include <Arca/Signal.h>
#include "Entity.h"

namespace Atmos::Entity
{
    struct MoveEntityDirection
    {
        Arca::Index<Entity> entity;
        Spatial::Direction direction;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Entity::MoveEntityDirection>
    {
        static const ObjectType objectType = ObjectType::Command;
        static inline const TypeName typeName = "Atmos::Entity::MoveEntityDirection";
    };
}