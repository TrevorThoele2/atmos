#pragma once

#include "AssetFactory.h"

#include "MaterialAsset.h"

namespace Atmos
{
    template<>
    class ObjectFactory<MaterialAsset> : public AssetFactory<MaterialAsset>
    {
    public:
        ObjectFactory(ObjectManager& manager);
    };
}