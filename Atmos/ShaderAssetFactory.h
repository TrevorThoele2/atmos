#pragma once

#include "AssetFactory.h"

#include "ShaderAsset.h"

namespace Atmos
{
    template<>
    class ObjectFactory<Asset::ShaderAsset> : public FileAssetFactory<Asset::ShaderAsset>
    {
    public:
        ObjectFactory(ObjectManager& manager);
    };
}