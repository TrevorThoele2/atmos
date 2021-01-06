#pragma once

#include <Arca/Command.h>
#include "Entity.h"

namespace Atmos::Entity
{
    struct ModifyData
    {
        Arca::Index<Entity> entity;
        std::vector<Datum> add;
        std::vector<String> remove;
        std::vector<Datum> replace;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Entity::ModifyData>
    {
        static const ObjectType objectType = ObjectType::Command;
        static TypeName TypeName() { return "Atmos::Entity::ModifyData"; }
    };
}