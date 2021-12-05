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
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Asset::FontCurator>
    {
        static constexpr ObjectType objectType = ObjectType::Curator;
        static const inline TypeName typeName = "Atmos::Asset::FontCurator";
        using HandledCommands = HandledCommands<
            Atmos::Asset::FindByName<Atmos::Asset::Font>,
            Atmos::Asset::Resource::LoadData<Atmos::Asset::Resource::Font>>;
    };
}

namespace Inscription
{
    template<class Format>
    struct ScribeTraits<Atmos::Asset::FontCurator, Format> final
    {
        using Category = AssetScribeCategory<Atmos::Asset::FontCurator>;
    };
}