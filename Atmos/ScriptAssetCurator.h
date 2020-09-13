#pragma once

#include "AssetCurator.h"
#include "ScriptAsset.h"

namespace Atmos::Asset
{
    using ScriptCurator = Curator<Script>;

    template<>
    struct CuratorTraits<Script> : CuratorTraitsBase<Script>
    {
        constexpr static DebugStatisticsSize debugStatisticsSize = &Debug::Statistics::Memory::scriptAssetSize;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Asset::ScriptCurator>
    {
        static const ObjectType objectType = ObjectType::Curator;
        static inline const TypeName typeName = "Atmos::Asset::ScriptCurator";
        using HandledCommands = HandledCommands<Atmos::Asset::Find<Atmos::Asset::Script>>;
    };
}

namespace Inscription
{
    template<class Archive>
    struct ScribeTraits<Atmos::Asset::ScriptCurator, Archive> final
    {
        using Category = AssetScribeCategory<Atmos::Asset::ScriptCurator>;
    };
}