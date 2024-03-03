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
        Resource::Loaded<Resource::Script> Handle(const Resource::LoadDataFromFile<Resource::Script>& command);
        Resource::Loaded<Resource::Script> Handle(const Resource::LoadDataFromMemory<Resource::Script>& command);
    };

    template<>
    struct CuratorTraits<Script> : CuratorTraitsBase<Script>
    {
        constexpr static DebugStatisticsSize debugStatisticsSize = &Debug::Statistics::Memory::scriptAssetSize;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Asset::ScriptCurator>
    {
        static const ObjectType objectType = ObjectType::Curator;
        static TypeName TypeName() { return "Atmos::Asset::ScriptCurator"; }
        using HandledCommands = HandledCommands<
            Atmos::Asset::FindByName<Atmos::Asset::Script>,
            Atmos::Asset::Resource::LoadDataFromFile<Atmos::Asset::Resource::Script>,
            Atmos::Asset::Resource::LoadDataFromMemory<Atmos::Asset::Resource::Script>>;
    };
}

namespace Inscription
{
    template<class Archive>
    struct ScribeTraits<Atmos::Asset::ScriptCurator, Archive> final
    {
        using Category = AssetScribeCategory<Atmos::Asset::ScriptCurator>;
    };
}