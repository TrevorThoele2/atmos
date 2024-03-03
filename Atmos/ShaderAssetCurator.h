#pragma once

#include "AssetCurator.h"
#include "ShaderAsset.h"

namespace Atmos::Asset
{
    using ShaderAssetCurator = AssetCurator<ShaderAsset>;

    template<>
    struct AssetCuratorTraits<ShaderAsset> : AssetCuratorTraitsBase<ShaderAsset>
    {
        constexpr static DebugStatisticsSize debugStatisticsSize = &Debug::Statistics::Memory::shaderAssetSize;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Asset::ShaderAssetCurator>
    {
        static const ObjectType objectType = ObjectType::Curator;
        static inline const TypeName typeName = "ShaderAssetCurator";
        using HandledCommands = HandledCommands<Atmos::Asset::FindAsset<Atmos::Asset::ShaderAsset>>;
    };
}

namespace Inscription
{
    template<>
    class Scribe<Atmos::Asset::ShaderAssetCurator, BinaryArchive> final :
        public ArcaNullScribe<Atmos::Asset::ShaderAssetCurator, BinaryArchive>
    {};
}