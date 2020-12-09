#pragma once

#include <Arca/Relic.h>

#include "InputID.h"
#include "Action.h"

namespace Atmos::Input
{
    class MappedActions final
    {
    public:
        std::unordered_map<InputID, Action> entries;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Input::MappedActions>
    {
        static const ObjectType objectType = ObjectType::Relic;
        static inline const TypeName typeName = "Atmos::Input::MappedActions";
        static const Locality locality = Locality::Global;
    };
}

namespace Inscription
{
    template<class Archive>
    struct ScribeTraits<Atmos::Input::MappedActions, Archive> final
    {
        using Category = ArcaNullScribeCategory<Atmos::Input::MappedActions>;
    };
}