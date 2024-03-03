
#include "ImageAssetSystem.h"

#include "DebugStatisticsSystem.h"

#include "StringUtility.h"

namespace Atmos
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
    void Scribe<::Atmos::ImageAssetSystem, BinaryArchive>::Scriven(ObjectT& object, ArchiveT& archive)
    {
        BaseScriven<::Atmos::AssetSystem<::Atmos::ImageAsset>>(object, archive);
    }
}