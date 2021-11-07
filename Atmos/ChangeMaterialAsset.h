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
        static constexpr ObjectType objectType = ObjectType::Command;
        static const inline TypeName typeName = "Atmos::Render::ChangeMaterialAsset";
    };
}