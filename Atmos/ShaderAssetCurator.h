#pragma once

#include "AssetCurator.h"
#include "ShaderAsset.h"
#include "GraphicsManager.h"

namespace Atmos::Asset
{
    using ShaderCurator = Curator<Shader>;

    template<>
    struct CuratorTraits<Shader> : CuratorTraitsBase<Shader>
    {
        constexpr static DebugStatisticsSize debugStatisticsSize = &Debug::Statistics::Memory::shaderAssetSize;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Asset::ShaderCurator>
    {
        static const ObjectType objectType = ObjectType::Curator;
        static inline const TypeName typeName = "ShaderAssetCurator";
        using HandledCommands = HandledCommands<
            Atmos::Asset::Find<Atmos::Asset::Shader>>;
    };
}

namespace Inscription
{
    template<>
    class Scribe<Atmos::Asset::ShaderCurator, BinaryArchive> final :
        public ArcaNullScribe<Atmos::Asset::ShaderCurator, BinaryArchive>
    {};
}