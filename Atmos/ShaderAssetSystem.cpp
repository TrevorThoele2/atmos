#include "ShaderAssetSystem.h"

#include "DebugStatisticsSystem.h"

#include "StringUtility.h"

namespace Atmos::Asset
{
    ShaderAssetSystem::ShaderAssetSystem(ObjectManager& manager) : AssetSystem(manager)
    {}

    void ShaderAssetSystem::InitializeImpl()
    {
        auto debugStatistics = Manager()->FindSystem<DebugStatisticsSystem>();
        debugStatistics->memoryPage.shaderAssetSize.retrievalFunction = [this]() -> String
        {
            return ToString(Size());
        };
    }
}

namespace Inscription
{
    void Scribe<::Atmos::Asset::ShaderAssetSystem, BinaryArchive>::ScrivenImplementation(ObjectT& object, ArchiveT& archive)
    {
        BaseScriven<::Atmos::Asset::AssetSystem<::Atmos::Asset::ShaderAsset>>(object, archive);
    }
}