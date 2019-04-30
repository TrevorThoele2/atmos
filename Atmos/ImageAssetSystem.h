
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
        ImageAssetSystem(const ::Inscription::Table<ImageAssetSystem>& table);
    private:
        void InitializeImpl() override;
    };
}

namespace Inscription
{
    INSCRIPTION_INSCRIPTER_DECLARE(::Atmos::ImageAssetSystem)
    {
    public:
        INSCRIPTION_INSCRIPTER_DECLARE_TABLE;
        INSCRIPTION_DECLARE_CLASS_NAME_RESOLVER;
    };
}