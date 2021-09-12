#pragma once

#include "ArcaRelicIncludes.h"
#include "Stopwatch.h"

namespace Atmos::Diagnostics
{
    struct Statistics
    {
        size_t relicCount;
        size_t shardCount;

        double renderTime;
        double idleTime;

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