#pragma once

#include <Arca/Command.h>
#include "Path.h"
#include "Entity.h"

namespace Atmos::Entity
{
    struct FindPath
    {
        Arca::Index<Entity> entity;
        Spatial::Grid::Point to;

        explicit FindPath(const Arca::Index<Entity>& entity, const Spatial::Grid::Point& to) :
            entity(entity), to(to)
        {}
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Entity::FindPath>
    {
        static const ObjectType objectType = ObjectType::Command;
        static TypeName TypeName() { return "Atmos::Entity::FindPath"; }
        using Result = Atmos::Entity::Path;
    };
}