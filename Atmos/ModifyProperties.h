#pragma once

#include <Arca/Command.h>
#include <Arca/RelicID.h>
#include "Property.h"

namespace Atmos
{
    struct ModifyProperties
    {
        Arca::RelicID id;
        std::vector<Property> add;
        std::vector<String> remove;
        std::vector<Property> replace;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::ModifyProperties>
    {
        static constexpr ObjectType objectType = ObjectType::Command;
        static const inline TypeName typeName = "Atmos::ModifyProperties";
    };
}