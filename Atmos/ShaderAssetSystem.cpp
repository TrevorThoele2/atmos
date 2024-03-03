
#include "ShaderAssetSystem.h"

#include "DebugStatisticsSystem.h"

#include "StringUtility.h"

namespace Atmos
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
    void Scribe<::Atmos::ShaderAssetSystem, BinaryArchive>::Scriven(ObjectT& object, ArchiveT& archive)
    {
        BaseScriven<::Atmos::AssetSystem<::Atmos::ShaderAsset>>(object, archive);
    }
}