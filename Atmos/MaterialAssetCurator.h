#pragma once

#include "AssetCurator.h"
#include "MaterialAsset.h"

namespace Atmos::Asset
{
    using MaterialCurator = Curator<Material>;

    template<>
    struct CuratorTraits<Material> : CuratorTraitsBase<Material>
    {
        constexpr static DebugStatisticsSize debugStatisticsSize = &Debug::Statistics::Memory::materialAssetSize;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Asset::MaterialCurator>
    {
        static const ObjectType objectType = ObjectType::Curator;
        static TypeName TypeName() { return "Atmos::Asset::MaterialCurator"; }
        using HandledCommands = HandledCommands<Atmos::Asset::FindByName<Atmos::Asset::Material>>;
    };
}

namespace Inscription
{
    template<class Archive>
    struct ScribeTraits<Atmos::Asset::MaterialCurator, Archive> final
    {
        using Category = AssetScribeCategory<Atmos::Asset::MaterialCurator>;
    };
}