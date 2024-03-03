#pragma once

#include <Arca/Command.h>

namespace Atmos::Entity
{
    struct ActualizeAllPrototypes
    {};
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Entity::ActualizeAllPrototypes>
    {
        static const ObjectType objectType = ObjectType::Command;
        static inline const TypeName typeName = "Atmos::Entity::ActualizeAllPrototypes";
    };
}