#pragma once

#include <Arca/Command.h>
#include "Property.h"

namespace Atmos::World
{
    struct RetrieveProperties
    {};
}

namespace Arca
{
    template<>
    struct Traits<Atmos::World::RetrieveProperties>
    {
        static const ObjectType objectType = ObjectType::Command;
        static TypeName TypeName() { return "Atmos::World::RetrieveProperties"; }
        using Result = std::vector<Atmos::Property>;
    };
}