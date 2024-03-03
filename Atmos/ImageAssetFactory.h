#pragma once

#include "AssetFactory.h"

#include "ImageAsset.h"

namespace Atmos
{
    template<>
    class ObjectFactory<ImageAsset> : public FileAssetFactory<ImageAsset>
    {
    public:
        ObjectFactory(ObjectManager& manager);
    };
}