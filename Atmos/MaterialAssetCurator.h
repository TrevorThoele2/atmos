#pragma once

#include "AssetCurator.h"
#include "MaterialAsset.h"

namespace Atmos::Asset
{
    using MaterialAssetCurator = AssetCurator<MaterialAsset>;

    template<>
    struct AssetCuratorTraits<MaterialAsset> : AssetCuratorTraitsBase<MaterialAsset>
    {
        constexpr static DebugStatisticsSize debugStatisticsSize = &Debug::Statistics::Memory::materialAssetSize;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Asset::MaterialAssetCurator>
    {
        static const ObjectType objectType = ObjectType::Curator;
        static inline const TypeName typeName = "MaterialAssetCurator";
        using HandledCommands = HandledCommands<Atmos::Asset::FindAsset<Atmos::Asset::MaterialAsset>>;
    };
}

namespace Inscription
{
    template<>
    class Scribe<Atmos::Asset::MaterialAssetCurator, BinaryArchive> final :
        public ArcaNullScribe<Atmos::Asset::MaterialAssetCurator, BinaryArchive>
    {};
}