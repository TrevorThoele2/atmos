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
        static constexpr ObjectType objectType = ObjectType::Relic;
        static const inline TypeName typeName = "Atmos::Logging::Information";
        static constexpr Locality locality = Locality::Global;
    };
}

namespace Inscription
{
    template<class Format>
    struct ScribeTraits<Atmos::Logging::Information, Format> final
    {
        using Category = ArcaNullScribeCategory<Atmos::Logging::Information>;
    };
}