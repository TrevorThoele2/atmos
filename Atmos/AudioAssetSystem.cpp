
#include "AudioAssetSystem.h"

#include "DebugStatisticsSystem.h"

#include "StringUtility.h"

namespace Atmos
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
    void Scribe<::Atmos::AudioAssetSystem, BinaryArchive>::Scriven(ObjectT& object, ArchiveT& archive)
    {
        BaseScriven<::Atmos::AssetSystem<::Atmos::AudioAsset>>(object, archive);
    }
}