#pragma once

#include "AssetFactory.h"

#include "ScriptAsset.h"

namespace Atmos
{
    template<>
    class ObjectFactory<ScriptAsset> : public FileAssetFactory<ScriptAsset>
    {
    public:
        ObjectFactory(ObjectManager& manager);
    };
}