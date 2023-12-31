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
        Resource::LoadedData<Resource::Shader> Handle(const Resource::LoadData<Resource::Shader>& command);
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Asset::ShaderCurator>
    {
        static constexpr ObjectType objectType = ObjectType::Curator;
        static const inline TypeName typeName = "Atmos::Asset::ShaderCurator";
        using HandledCommands = HandledCommands<
            Atmos::Asset::FindByName<Atmos::Asset::Shader>,
            Atmos::Asset::Resource::LoadData<Atmos::Asset::Resource::Shader>>;
    };
}

namespace Inscription
{
    template<class Format>
    struct ScribeTraits<Atmos::Asset::ShaderCurator, Format> final
    {
        using Category = AssetScribeCategory<Atmos::Asset::ShaderCurator>;
    };
}