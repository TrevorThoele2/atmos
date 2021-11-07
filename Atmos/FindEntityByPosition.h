#pragma once

#include <Arca/Command.h>
#include "Entity.h"
#include <Arca/Index.h>
#include <Arca/TypeFor.h>

namespace Atmos::Entity
{
    struct FindByPosition
    {
        Spatial::Grid::Point position;

        explicit FindByPosition(const Spatial::Grid::Point& position) : position(position)
        {}
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Entity::FindByPosition>
    {
        static constexpr ObjectType objectType = ObjectType::Command;
        static const inline TypeName typeName = "Atmos::Entity::FindByPosition";
        using Result = std::set<Index<Atmos::Entity::Entity>>;
    };
}