
#include "ScriptAssetSystem.h"

#include "DebugStatisticsSystem.h"

#include "StringUtility.h"

namespace Atmos::Asset
{
    ScriptAssetSystem::ScriptAssetSystem(ObjectManager& manager) : AssetSystem(manager)
    {}

    void ScriptAssetSystem::InitializeImpl()
    {
        auto debugStatistics = Manager()->FindSystem<DebugStatisticsSystem>();
        debugStatistics->memoryPage.scriptAssetSize.retrievalFunction = [this]() -> String
        {
            return ToString(Size());
        };
    }
}

namespace Inscription
{
    void Scribe<::Atmos::Asset::ScriptAssetSystem, BinaryArchive>::ScrivenImplementation(ObjectT& object, ArchiveT& archive)
    {
        BaseScriven<::Atmos::Asset::AssetSystem<::Atmos::Asset::ScriptAsset>>(object, archive);
    }
}