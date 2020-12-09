#pragma once

#include <Arca/Command.h>
#include "Entity.h"

namespace Atmos::Entity
{
    struct ModifyTags
    {
        Arca::Index<Entity> entity;
        std::vector<String> add;
        std::vector<String> remove;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Entity::ModifyTags>
    {
        static const ObjectType objectType = ObjectType::Command;
        static inline const TypeName typeName = "Atmos::Entity::ModifyTags";
    };
}