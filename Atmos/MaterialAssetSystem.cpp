
#include "MaterialAssetSystem.h"

#include "DebugStatisticsSystem.h"

#include "StringUtility.h"

namespace Atmos::Asset
{
    MaterialAssetSystem::MaterialAssetSystem(ObjectManager& manager) : AssetSystem(manager)
    {}

    void MaterialAssetSystem::InitializeImpl()
    {
        auto debugStatistics = Manager()->FindSystem<DebugStatisticsSystem>();
        debugStatistics->memoryPage.materialAssetSize.retrievalFunction = [this]() -> String
        {
            return ToString(Size());
        };
    }
}

namespace Inscription
{
    void Scribe<::Atmos::Asset::MaterialAssetSystem, BinaryArchive>::ScrivenImplementation(ObjectT& object, ArchiveT& archive)
    {
        BaseScriven<::Atmos::Asset::AssetSystem<::Atmos::Asset::MaterialAsset>>(object, archive);
    }
}