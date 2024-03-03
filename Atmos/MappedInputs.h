#pragma once

#include <Arca/ClosedTypedRelicAutomation.h>

#include "InputID.h"
#include "Input.h"

namespace Atmos::Input
{
    class MappedInputs final : public Arca::ClosedTypedRelicAutomation<MappedInputs>
    {
    public:
        std::unordered_map<InputID, Input> entries;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Input::MappedInputs>
    {
        static const ObjectType objectType = ObjectType::Relic;
        static inline const TypeName typeName = "MappedInputs";
        static const Locality locality = Locality::Global;
    };
}

namespace Inscription
{
    template<>
    class Scribe<Atmos::Input::MappedInputs, BinaryArchive> final
        : public ArcaNullScribe<Atmos::Input::MappedInputs, BinaryArchive>
    {};
}