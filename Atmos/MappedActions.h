#pragma once

#include <Arca/ClosedTypedRelicAutomation.h>

#include "InputID.h"
#include "Action.h"

namespace Atmos::Input
{
    class MappedActions final : public Arca::ClosedTypedRelicAutomation<MappedActions>
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
        static inline const TypeName typeName = "MappedActions";
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