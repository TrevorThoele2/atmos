#pragma once

#include <Arca/Command.h>

#include "Entity.h"

namespace Atmos::Entity
{
    struct SetAvatar
    {
        Arca::Index<Entity> setAs;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Entity::SetAvatar>
    {
        static const ObjectType objectType = ObjectType::Command;
        static const inline TypeName typeName = "Atmos::Entity::SetAvatar";
    };
}