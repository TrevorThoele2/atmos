#pragma once

#include "AssetCurator.h"
#include "AudioAsset.h"

namespace Atmos::Asset
{
    using AudioCurator = Curator<Audio>;

    template<>
    struct CuratorTraits<Audio> : CuratorTraitsBase<Audio>
    {
        constexpr static DebugStatisticsSize debugStatisticsSize = &Debug::Statistics::Memory::audioAssetSize;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Asset::AudioCurator>
    {
        static const ObjectType objectType = ObjectType::Curator;
        static inline const TypeName typeName = "Atmos::Asset::AudioCurator";
        using HandledCommands = HandledCommands<Atmos::Asset::FindByName<Atmos::Asset::Audio>>;
    };
}

namespace Inscription
{
    template<class Archive>
    struct ScribeTraits<Atmos::Asset::AudioCurator, Archive> final
    {
        using Category = AssetScribeCategory<Atmos::Asset::AudioCurator>;
    };
}