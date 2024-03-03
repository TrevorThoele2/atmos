
#pragma once

#include "AssetSystem.h"

#include "ImageAsset.h"

#include "Serialization.h"

namespace Atmos
{
    class ImageAssetSystem : public AssetSystem<ImageAsset>
    {
    public:
        ImageAssetSystem(ObjectManager& manager);
        INSCRIPTION_BINARY_TABLE_CONSTRUCTOR_DECLARE(ImageAssetSystem);
    private:
        void InitializeImpl() override;
    };
}

namespace Inscription
{
    INSCRIPTION_INSCRIPTER_DECLARE(::Atmos::ImageAssetSystem)
    {
    public:
        INSCRIPTION_BINARY_INSCRIPTER_DECLARE_TABLE;
        INSCRIPTION_BINARY_DECLARE_CLASS_NAME_RESOLVER;
    };
}