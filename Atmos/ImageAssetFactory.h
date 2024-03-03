#pragma once

#include "AssetFactory.h"

#include "ImageAsset.h"

namespace Atmos
{
    template<>
    class ObjectFactory<Asset::ImageAsset> : public FileAssetFactory<Asset::ImageAsset>
    {
    public:
        ObjectFactory(ObjectManager& manager);
    };
}