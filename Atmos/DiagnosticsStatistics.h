#pragma once

#include "ArcaRelicIncludes.h"
#include "Stopwatch.h"

namespace Atmos::Diagnostics
{
    struct Statistics
    {
        size_t relicCount = 0;
        size_t shardCount = 0;
        Arca::RelicID nextRelicID = 0;

        double renderTime = 0.0;
        double idleTime = 0.0;

        bool operator==(const Statistics& arg) const;
        bool operator!=(const Statistics& arg) const;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Diagnostics::Statistics>
    {
        static const ObjectType objectType = ObjectType::Relic;
        static TypeName TypeName() { return "Atmos::Diagnostics::Statistics"; }
        static const Locality locality = Locality::Global;
    };
}

namespace Inscription
{
    template<class Archive>
    struct ScribeTraits<Atmos::Diagnostics::Statistics, Archive> final
    {
        using Category = ArcaNullScribeCategory<Atmos::Diagnostics::Statistics>;
    };
}