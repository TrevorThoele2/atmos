#pragma once

#include <Arca/ClosedTypedRelic.h>
#include "FieldID.h"

namespace Atmos::World
{
    class CurrentFieldData final : public Arca::ClosedTypedRelic<CurrentFieldData>
    {
    public:
        FieldID fieldID = 0;
    public:
        explicit CurrentFieldData(Init init);
        CurrentFieldData(Init init, FieldID fieldID);
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::World::CurrentFieldData>
    {
        static const ObjectType objectType = ObjectType::Relic;
        static inline const TypeName typeName = "Atmos::World::CurrentFieldData";
        static const Locality locality = Locality::Global;
    };
}

namespace Inscription
{
    template<class Archive>
    struct ScribeTraits<Atmos::World::CurrentFieldData, Archive> final
    {
        using Category = ArcaNullScribeCategory<Atmos::World::CurrentFieldData>;
    };
}