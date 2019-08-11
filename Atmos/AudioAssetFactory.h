#pragma once

#include "AssetFactory.h"

#include "AudioAsset.h"

namespace Atmos
{
    template<>
    class ObjectFactory<Asset::AudioAsset> : public FileAssetFactory<Asset::AudioAsset>
    {
    public:
        ObjectFactory(ObjectManager& manager);
    };
}