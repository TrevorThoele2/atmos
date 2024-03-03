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
        static constexpr ObjectType objectType = ObjectType::Relic;
        static const inline TypeName typeName = "Atmos::World::CurrentFieldData";
        static constexpr Locality locality = Locality::Global;
    };
}

namespace Inscription
{
    template<class Format>
    struct ScribeTraits<Atmos::World::CurrentFieldData, Format> final
    {
        using Category = ArcaNullScribeCategory<Atmos::World::CurrentFieldData>;
    };
}