#pragma once

#include <Arca/Relic.h>

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
        static const ObjectType objectType = ObjectType::Relic;
        static inline const TypeName typeName = "Atmos::Scripting::CurrentExecutingScript";
        static const Locality locality = Locality::Global;
    };
}

namespace Inscription
{
    template<class Archive>
    struct ScribeTraits<Atmos::Scripting::CurrentExecutingScript, Archive> final
    {
        using Category = ArcaNullScribeCategory<Atmos::Scripting::CurrentExecutingScript>;
    };
}