#pragma once

#include "AssetCurator.h"
#include "ShaderAsset.h"
#include "LoadShaderAssetResourceData.h"
#include "GraphicsManager.h"

namespace Atmos::Asset
{
    class ShaderCurator final : public Curator<Shader>
    {
    public:
        using Curator::Curator;

        using Curator::Handle;
        Resource::Loaded<Resource::Shader> Handle(const Resource::LoadDataFromFile<Resource::Shader>& command);
        Resource::Loaded<Resource::Shader> Handle(const Resource::LoadDataFromMemory<Resource::Shader>& command);
    };

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
            Atmos::Asset::FindByName<Atmos::Asset::Shader>,
            Atmos::Asset::Resource::LoadDataFromFile<Atmos::Asset::Resource::Shader>,
            Atmos::Asset::Resource::LoadDataFromMemory<Atmos::Asset::Resource::Shader>>;
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