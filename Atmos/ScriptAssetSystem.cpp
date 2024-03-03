
#include "ScriptAssetSystem.h"

#include "DebugStatisticsSystem.h"

#include "StringUtility.h"

namespace Atmos
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
    void Scribe<::Atmos::ScriptAssetSystem, BinaryArchive>::Scriven(ObjectT& object, ArchiveT& archive)
    {
        BaseScriven<::Atmos::AssetSystem<::Atmos::ScriptAsset>>(object, archive);
    }
}