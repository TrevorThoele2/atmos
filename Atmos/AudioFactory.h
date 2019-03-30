#pragma once

#include "AssetFactory.h"

#include "AudioAsset.h"

namespace Atmos
{
    template<>
    class ObjectFactory<AudioAsset> : public FileAssetFactory<AudioAsset>
    {
    public:
        ObjectFactory(ObjectManager& manager);
    };
}