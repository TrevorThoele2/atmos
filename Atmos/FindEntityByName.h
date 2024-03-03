#pragma once

#include <Arca/Command.h>
#include "String.h"
#include "Entity.h"
#include <Arca/Index.h>
#include <Arca/TypeFor.h>

namespace Atmos::Entity
{
    struct FindByName
    {
        String name;

        explicit FindByName(const String& name) : name(name)
        {}
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Entity::FindByName>
    {
        static const ObjectType objectType = ObjectType::Command;
        static inline const TypeName typeName = "Atmos::Entity::FindByName";
        using Result = Index<Atmos::Entity::Entity>;
    };
}