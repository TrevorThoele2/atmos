
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
    private:
        void InitializeImpl() override;
    };
}

namespace Inscription
{
    template<>
    class Scribe<::Atmos::ImageAssetSystem, BinaryArchive> :
        public ObjectSystemScribe<::Atmos::ImageAssetSystem, BinaryArchive>
    {
    public:
        static void Scriven(ObjectT& object, ArchiveT& archive);
    };
}