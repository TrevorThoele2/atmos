#pragma once

#include <Arca/Command.h>
#include "Property.h"

namespace Atmos::World
{
    struct ModifyProperties
    {
        std::vector<Property> add;
        std::vector<String> remove;
        std::vector<Property> replace;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::World::ModifyProperties>
    {
        static constexpr ObjectType objectType = ObjectType::Command;
        static const inline TypeName typeName = "Atmos::World::ModifyProperties";
    };
}