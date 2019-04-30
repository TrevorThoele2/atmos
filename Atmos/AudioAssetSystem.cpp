
#include "AudioAssetSystem.h"

#include "DebugStatisticsSystem.h"

#include "StringUtility.h"

namespace Atmos
{
    AudioAssetSystem::AudioAssetSystem(ObjectManager& manager) : AssetSystem(manager)
    {}

    AudioAssetSystem::AudioAssetSystem(const ::Inscription::Table<AudioAssetSystem>& table) : INSCRIPTION_TABLE_GET_BASE(AssetSystem)
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
    INSCRIPTION_INSCRIPTER_DEFINE_TABLE(::Atmos::AudioAssetSystem)
    {
        INSCRIPTION_INSCRIPTER_CREATE_TABLE;

        INSCRIPTION_TABLE_ADD_BASE(::Atmos::AssetSystem<::Atmos::AudioAsset>);

        INSCRIPTION_INSCRIPTER_RETURN_TABLE;
    }

    INSCRIPTION_DEFINE_SIMPLE_CLASS_NAME_RESOLVER(::Atmos::AudioAssetSystem, "AudioAssetSystem");
}