#pragma once

#include "AssetCurator.h"
#include "ActionAsset.h"

namespace Atmos::Asset
{
    using ActionCurator = Curator<Action>;

    template<>
    struct CuratorTraits<Action> : CuratorTraitsBase<Action>
    {
        constexpr static DebugStatisticsSize debugStatisticsSize = &Debug::Statistics::Memory::materialAssetSize;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Asset::ActionCurator>
    {
        static const ObjectType objectType = ObjectType::Curator;
        static inline const TypeName typeName = "Atmos::Asset::ActionCurator";
        using HandledCommands = HandledCommands<Atmos::Asset::FindByName<Atmos::Asset::Action>>;
    };
}

namespace Inscription
{
    template<class Archive>
    struct ScribeTraits<Atmos::Asset::ActionCurator, Archive> final
    {
        using Category = AssetScribeCategory<Atmos::Asset::ActionCurator>;
    };
}