#pragma once

#include "AssetCurator.h"
#include "LineMaterialAsset.h"

namespace Atmos::Asset
{
    using LineMaterialCurator = Curator<LineMaterial>;

    template<>
    struct CuratorTraits<LineMaterial> : CuratorTraitsBase<LineMaterial>
    {
        constexpr static DebugStatisticsSize debugStatisticsSize = &Debug::Statistics::Memory::materialAssetSize;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Asset::LineMaterialCurator>
    {
        static const ObjectType objectType = ObjectType::Curator;
        static inline const TypeName typeName = "Atmos::Asset::LineMaterialCurator";
        using HandledCommands = HandledCommands<Atmos::Asset::FindByName<Atmos::Asset::LineMaterial>>;
    };
}

namespace Inscription
{
    template<class Archive>
    struct ScribeTraits<Atmos::Asset::LineMaterialCurator, Archive> final
    {
        using Category = AssetScribeCategory<Atmos::Asset::LineMaterialCurator>;
    };
}