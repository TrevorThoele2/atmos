#pragma once

#include "AssetCurator.h"
#include "ActionAsset.h"

namespace Atmos::Asset
{
    using ActionCurator = Curator<Action>;
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Asset::ActionCurator>
    {
        static constexpr ObjectType objectType = ObjectType::Curator;
        static const inline TypeName typeName = "Atmos::Asset::ActionCurator";
        using HandledCommands = HandledCommands<Atmos::Asset::FindByName<Atmos::Asset::Action>>;
    };
}

namespace Inscription
{
    template<class Archive>
    struct ScribeTraits<Atmos::Asset::ActionCurator, Archive> final
    {
        using Category = AssetScribeCategory<Atmos::Asset::ActionCurator>;
    };
}