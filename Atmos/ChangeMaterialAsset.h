#pragma once

#include <Arca/Command.h>

#include "ImageMaterialAsset.h"
#include "LineMaterialAsset.h"
#include "RegionMaterialAsset.h"

namespace Atmos::Render
{
    template<class Material>
    struct ChangeMaterialAsset
    {
        Arca::RelicID id = 0;

        Arca::Index<Material> to;
    };

    using ChangeImageMaterialAsset = ChangeMaterialAsset<Asset::ImageMaterial>;
    using ChangeLineMaterialAsset = ChangeMaterialAsset<Asset::LineMaterial>;
    using ChangeRegionMaterialAsset = ChangeMaterialAsset<Asset::RegionMaterial>;
}

namespace Arca
{
    template<class Material>
    struct Traits<Atmos::Render::ChangeMaterialAsset<Material>>
    {
        static const ObjectType objectType = ObjectType::Command;
        static inline const TypeName typeName = "Atmos::Render::ChangeMaterialAsset<" + Traits<Material>::typeName + ">";
    };
}