#pragma once

#include "AssetCurator.h"
#include "ScriptAsset.h"

namespace Atmos::Asset
{
    using ScriptAssetCurator = AssetCurator<ScriptAsset>;

    template<>
    struct AssetCuratorTraits<ScriptAsset> : AssetCuratorTraitsBase<ScriptAsset>
    {
        constexpr static DebugStatisticsSize debugStatisticsSize = &Debug::Statistics::Memory::scriptAssetSize;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Asset::ScriptAssetCurator>
    {
        static const ObjectType objectType = ObjectType::Curator;
        static inline const TypeName typeName = "ScriptAssetCurator";
        using HandledCommands = HandledCommands<Atmos::Asset::FindAsset<Atmos::Asset::ScriptAsset>>;
    };
}

namespace Inscription
{
    template<>
    class Scribe<Atmos::Asset::ScriptAssetCurator, BinaryArchive> final :
        public ArcaNullScribe<Atmos::Asset::ScriptAssetCurator, BinaryArchive>
    {};
}