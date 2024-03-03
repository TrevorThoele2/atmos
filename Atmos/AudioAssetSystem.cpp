
#include "AudioAssetSystem.h"

#include "DebugStatisticsSystem.h"

#include "StringUtility.h"

namespace Atmos::Asset
{
    AudioAssetSystem::AudioAssetSystem(ObjectManager& manager) : AssetSystem(manager)
    {}

    void AudioAssetSystem::InitializeImpl()
    {
        auto debugStatistics = Manager()->FindSystem<DebugStatisticsSystem>();
        debugStatistics->memoryPage.audioAssetSize.retrievalFunction = [this]() -> String
        {
            return ToString(Size());
        };
    }
}

namespace Inscription
{
    void Scribe<::Atmos::Asset::AudioAssetSystem, BinaryArchive>::ScrivenImplementation(ObjectT& object, ArchiveT& archive)
    {
        BaseScriven<::Atmos::Asset::AssetSystem<::Atmos::Asset::AudioAsset>>(object, archive);
    }
}