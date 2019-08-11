#pragma once

#include "AssetFactory.h"

#include "MaterialAsset.h"

namespace Atmos
{
    template<>
    class ObjectFactory<Asset::MaterialAsset> : public AssetFactory<Asset::MaterialAsset>
    {
    public:
        ObjectFactory(ObjectManager& manager);
    };
}