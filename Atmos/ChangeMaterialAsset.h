#pragma once

#include <Arca/Command.h>

#include "MaterialAsset.h"

namespace Atmos::Render
{
    struct ChangeMaterialAsset
    {
        Arca::RelicID id = 0;

        Arca::Index<Asset::Material> to;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Render::ChangeMaterialAsset>
    {
        static const ObjectType objectType = ObjectType::Command;
        static TypeName TypeName() { return "Atmos::Render::ChangeMaterialAsset"; }
    };
}