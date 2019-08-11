
#pragma once

#include "AssetSystem.h"

#include "ImageAsset.h"

#include "Serialization.h"

namespace Atmos::Asset
{
    class ImageAssetSystem : public AssetSystem<ImageAsset>
    {
    public:
        ImageAssetSystem(ObjectManager& manager);
    protected:
        void InitializeImpl() override;
    };
}

namespace Inscription
{
    template<>
    class Scribe<::Atmos::Asset::ImageAssetSystem, BinaryArchive> :
        public ObjectSystemScribe<::Atmos::Asset::ImageAssetSystem, BinaryArchive>
    {
    protected:
        void ScrivenImplementation(ObjectT& object, ArchiveT& archive) override;
    };
}