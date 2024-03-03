#pragma once

#include "AssetCurator.h"
#include "ScriptAsset.h"
#include "LoadScriptAssetResourceData.h"

namespace Atmos::Asset
{
    class ScriptCurator final : public Curator<Script>
    {
    public:
        using Curator::Curator;

        using Curator::Handle;
        Resource::LoadedData<Resource::Script> Handle(const Resource::LoadData<Resource::Script>& command);
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Asset::ScriptCurator>
    {
        static constexpr ObjectType objectType = ObjectType::Curator;
        static const inline TypeName typeName = "Atmos::Asset::ScriptCurator";
        using HandledCommands = HandledCommands<
            Atmos::Asset::FindByName<Atmos::Asset::Script>,
            Atmos::Asset::Resource::LoadData<Atmos::Asset::Resource::Script>>;
    };
}

namespace Inscription
{
    template<class Format>
    struct ScribeTraits<Atmos::Asset::ScriptCurator, Format> final
    {
        using Category = AssetScribeCategory<Atmos::Asset::ScriptCurator>;
    };
}