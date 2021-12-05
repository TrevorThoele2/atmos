#pragma once

#include "ArcaRelicIncludes.h"

namespace Atmos::Scripting
{
    class CurrentExecutingScript final
    {
    public:
        Arca::RelicID id;

        CurrentExecutingScript() = default;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Scripting::CurrentExecutingScript>
    {
        static constexpr ObjectType objectType = ObjectType::Relic;
        static const inline TypeName typeName = "Atmos::Scripting::CurrentExecutingScript";
        static constexpr Locality locality = Locality::Global;
    };
}

namespace Inscription
{
    template<class Format>
    struct ScribeTraits<Atmos::Scripting::CurrentExecutingScript, Format> final
    {
        using Category = ArcaNullScribeCategory<Atmos::Scripting::CurrentExecutingScript>;
    };
}