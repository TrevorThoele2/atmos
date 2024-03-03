#pragma once

#include <Arca/ClosedTypedRelic.h>

#include "InputID.h"
#include "Action.h"

namespace Atmos::Input
{
    class MappedActions final : public Arca::ClosedTypedRelic<MappedActions>
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
    template<>
    class Scribe<Atmos::Input::MappedActions, BinaryArchive> final
        : public ArcaNullScribe<Atmos::Input::MappedActions, BinaryArchive>
    {};
}