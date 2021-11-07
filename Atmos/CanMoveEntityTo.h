#pragma once

#include <Arca/Command.h>
#include "Entity.h"

namespace Atmos::Entity
{
    struct CanMoveTo
    {
        Arca::Index<Entity> entity;
        Spatial::Grid::Point to;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Entity::CanMoveTo>
    {
        static constexpr ObjectType objectType = ObjectType::Command;
        static const inline TypeName typeName = "Atmos::Entity::CanMoveTo";
        using Result = bool;
    };
}