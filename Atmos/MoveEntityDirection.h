#pragma once

#include <Arca/Signal.h>
#include "Entity.h"

namespace Atmos::Entity
{
    struct MoveEntityDirection
    {
        Arca::LocalPtr<Entity> entity;
        Direction direction;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Entity::MoveEntityDirection>
    {
        static const ObjectType objectType = ObjectType::Signal;
        static inline const TypeName typeName = "MoveEntityDirection";
    };
}