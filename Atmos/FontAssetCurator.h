#pragma once

#include "AssetCurator.h"
#include "FontAsset.h"

namespace Atmos::Asset
{
    class FontCurator : public Curator<Font>
    {
    public:
        using Curator::Curator;

        using Curator::Handle;
        Resource::LoadedData<Resource::Font> Handle(const Resource::LoadData<Resource::Font>& command);
    };

    template<>
    struct CuratorTraits<Font> : CuratorTraitsBase<Font>
    {
        constexpr static DebugStatisticsSize debugStatisticsSize = &Debug::Statistics::Memory::materialAssetSize;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Asset::FontCurator>
    {
        static const ObjectType objectType = ObjectType::Curator;
        static TypeName TypeName() { return "Atmos::Asset::FontCurator"; }
        using HandledCommands = HandledCommands<
            Atmos::Asset::FindByName<Atmos::Asset::Font>,
            Atmos::Asset::Resource::LoadData<Atmos::Asset::Resource::Font>>;
    };
}

namespace Inscription
{
    template<class Archive>
    struct ScribeTraits<Atmos::Asset::FontCurator, Archive> final
    {
        using Category = AssetScribeCategory<Atmos::Asset::FontCurator>;
    };
}