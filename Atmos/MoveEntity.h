#pragma once

#include <Arca/Command.h>
#include "Entity.h"

namespace Atmos::Entity
{
    struct Move
    {
        Arca::Index<Entity> entity;
        Spatial::Grid::Point toPosition;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Entity::Move>
    {
        static const ObjectType objectType = ObjectType::Command;
        static inline const TypeName typeName = "Atmos::Entity::MoveEntity";
    };
}