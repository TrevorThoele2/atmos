#pragma once

#include <Arca/Command.h>
#include "Entity.h"

namespace Atmos::Entity
{
    struct ModifyTags
    {
        Arca::RelicID entity;
        std::vector<String> add;
        std::vector<String> remove;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Entity::ModifyTags>
    {
        static constexpr ObjectType objectType = ObjectType::Command;
        static const inline TypeName typeName = "Atmos::Entity::ModifyTags";
    };
}