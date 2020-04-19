#pragma once

#include "AssetCurator.h"
#include "AudioAsset.h"

namespace Atmos::Asset
{
    using AudioAssetCurator = AssetCurator<AudioAsset>;

    template<>
    struct AssetCuratorTraits<AudioAsset> : AssetCuratorTraitsBase<AudioAsset>
    {
        constexpr static DebugStatisticsSize debugStatisticsSize = &Debug::Statistics::Memory::audioAssetSize;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Asset::AudioAssetCurator>
    {
        static const ObjectType objectType = ObjectType::Curator;
        static inline const TypeName typeName = "AudioAssetCurator";
        using HandledCommands = HandledCommands<Atmos::Asset::FindAsset<Atmos::Asset::AudioAsset>>;
    };
}

namespace Inscription
{
    template<>
    class Scribe<Atmos::Asset::AudioAssetCurator, BinaryArchive> final :
        public ArcaNullScribe<Atmos::Asset::AudioAssetCurator, BinaryArchive>
    {};
}