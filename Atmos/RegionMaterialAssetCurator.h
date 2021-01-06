#pragma once

#include "AssetCurator.h"
#include "RegionMaterialAsset.h"

namespace Atmos::Asset
{
    using RegionMaterialCurator = Curator<RegionMaterial>;

    template<>
    struct CuratorTraits<RegionMaterial> : CuratorTraitsBase<RegionMaterial>
    {
        constexpr static DebugStatisticsSize debugStatisticsSize = &Debug::Statistics::Memory::materialAssetSize;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Asset::RegionMaterialCurator>
    {
        static const ObjectType objectType = ObjectType::Curator;
        static TypeName TypeName() { return "Atmos::Asset::RegionMaterialCurator"; }
        using HandledCommands = HandledCommands<Atmos::Asset::FindByName<Atmos::Asset::RegionMaterial>>;
    };
}

namespace Inscription
{
    template<class Archive>
    struct ScribeTraits<Atmos::Asset::RegionMaterialCurator, Archive> final
    {
        using Category = AssetScribeCategory<Atmos::Asset::RegionMaterialCurator>;
    };
}