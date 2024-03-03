
#include "MaterialAssetSystem.h"

#include "DebugStatisticsSystem.h"

#include "StringUtility.h"

namespace Atmos
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
    void Scribe<::Atmos::MaterialAssetSystem, BinaryArchive>::Scriven(ObjectT& object, ArchiveT& archive)
    {
        BaseScriven<::Atmos::AssetSystem<::Atmos::MaterialAsset>>(object, archive);
    }
}