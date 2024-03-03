#pragma once

#include <Arca/Signal.h>
#include "Entity.h"
#include "Direction.h"

namespace Atmos::Entity
{
    struct RotateEntity
    {
        Arca::Index<Entity> entity;
        Spatial::Direction toFace;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Entity::RotateEntity>
    {
        static const ObjectType objectType = ObjectType::Command;
        static inline const TypeName typeName = "Atmos::Entity::RotateEntity";
    };
}