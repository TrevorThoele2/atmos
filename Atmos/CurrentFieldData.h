#pragma once

#include "ArcaRelicIncludes.h"
#include "FieldID.h"

namespace Atmos::World
{
    class CurrentFieldData final
    {
    public:
        FieldID fieldID = 0;
    public:
        CurrentFieldData() = default;
        explicit CurrentFieldData(FieldID fieldID);
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::World::CurrentFieldData>
    {
        static const ObjectType objectType = ObjectType::Relic;
        static TypeName TypeName() { return "Atmos::World::CurrentFieldData"; }
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