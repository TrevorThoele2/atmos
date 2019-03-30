#pragma once

#include "AssetFactory.h"

#include "ShaderAsset.h"

namespace Atmos
{
    template<>
    class ObjectFactory<ShaderAsset> : public FileAssetFactory<ShaderAsset>
    {
    public:
        ObjectFactory(ObjectManager& manager);
    };
}