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
        static inline const TypeName typeName = "Atmos::Asset::ShaderCurator";
        using HandledCommands = HandledCommands<
            Atmos::Asset::Find<Atmos::Asset::Shader>>;
    };
}

namespace Inscription
{
    template<class Archive>
    struct ScribeTraits<Atmos::Asset::ShaderCurator, Archive> final
    {
        using Category = AssetScribeCategory<Atmos::Asset::ShaderCurator>;
    };
}