#pragma once

#include <Arca/Command.h>
#include "Entity.h"

namespace Atmos::Entity
{
    struct MoveTo
    {
        Arca::Index<Entity> entity;
        Spatial::Grid::Point to;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Entity::MoveTo>
    {
        static const ObjectType objectType = ObjectType::Command;
        static TypeName TypeName() { return "Atmos::Entity::MoveTo"; }
    };
}