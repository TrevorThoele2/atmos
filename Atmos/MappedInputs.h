#pragma once

#include <Arca/ClosedTypedRelic.h>

#include "InputID.h"
#include "Input.h"

namespace Atmos::Input
{
    class MappedInputs final : public Arca::ClosedTypedRelic<MappedInputs>
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
        static inline const TypeName typeName = "Atmos::Input::MappedInputs";
        static const Locality locality = Locality::Global;
    };
}

namespace Inscription
{
    template<class Archive>
    struct ScribeTraits<Atmos::Input::MappedInputs, Archive> final
    {
        using Category = ArcaNullScribeCategory<Atmos::Input::MappedInputs>;
    };
}