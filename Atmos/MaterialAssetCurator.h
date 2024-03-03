#pragma once

#include "AssetCurator.h"
#include "MaterialAsset.h"

namespace Atmos::Asset
{
    using MaterialCurator = Curator<Material>;
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Asset::MaterialCurator>
    {
        static constexpr ObjectType objectType = ObjectType::Curator;
        static const inline TypeName typeName = "Atmos::Asset::MaterialCurator";
        using HandledCommands = HandledCommands<Atmos::Asset::FindByName<Atmos::Asset::Material>>;
    };
}

namespace Inscription
{
    template<class Format>
    struct ScribeTraits<Atmos::Asset::MaterialCurator, Format> final
    {
        using Category = AssetScribeCategory<Atmos::Asset::MaterialCurator>;
    };
}