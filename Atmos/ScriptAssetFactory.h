#pragma once

#include "AssetFactory.h"

#include "ScriptAsset.h"

namespace Atmos
{
    template<>
    class ObjectFactory<Asset::ScriptAsset> : public FileAssetFactory<Asset::ScriptAsset>
    {
    public:
        ObjectFactory(ObjectManager& manager);
    };
}