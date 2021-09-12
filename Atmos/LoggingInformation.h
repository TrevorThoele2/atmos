#pragma once

#include "ArcaRelicIncludes.h"
#include "Logger.h"

namespace Atmos::Logging
{
    class Information final
    {
    public:
        Logger* logger = nullptr;

        explicit Information(Logger& logger);
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Logging::Information>
    {
        static const ObjectType objectType = ObjectType::Relic;
        static TypeName TypeName() { return "Atmos::Logging::Information"; }
        static const Locality locality = Locality::Global;
    };
}

namespace Inscription
{
    template<class Archive>
    struct ScribeTraits<Atmos::Logging::Information, Archive> final
    {
        using Category = ArcaNullScribeCategory<Atmos::Logging::Information>;
    };
}