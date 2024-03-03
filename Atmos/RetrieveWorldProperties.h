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
        static constexpr ObjectType objectType = ObjectType::Command;
        static const inline TypeName typeName = "Atmos::World::RetrieveProperties";
        using Result = std::vector<Atmos::Property>;
    };
}