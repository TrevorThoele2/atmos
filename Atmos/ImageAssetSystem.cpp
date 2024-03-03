#include "ImageAssetSystem.h"

#include "DebugStatisticsSystem.h"

#include "StringUtility.h"

namespace Atmos::Asset
{
    ImageAssetSystem::ImageAssetSystem(ObjectManager& manager) : AssetSystem(manager)
    {}

    void ImageAssetSystem::InitializeImpl()
    {
        auto debugStatistics = Manager()->FindSystem<DebugStatisticsSystem>();
        debugStatistics->memoryPage.imageAssetSize.retrievalFunction = [this]() -> String
        {
            return ToString(Size());
        };
    }
}

namespace Inscription
{
    void Scribe<::Atmos::Asset::ImageAssetSystem, BinaryArchive>::ScrivenImplementation(ObjectT& object, ArchiveT& archive)
    {
        BaseScriven<::Atmos::Asset::AssetSystem<::Atmos::Asset::ImageAsset>>(object, archive);
    }
}