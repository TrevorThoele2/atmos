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
        static inline const TypeName typeName = "Atmos::Asset::MaterialCurator";
        using HandledCommands = HandledCommands<Atmos::Asset::Find<Atmos::Asset::Material>>;
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