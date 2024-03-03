#pragma once

#include <Arca/ClosedTypedRelicAutomation.h>

#include "TimeValue.h"
#include "Fps.h"

namespace Atmos::Time
{
    class Settings final : public Arca::ClosedTypedRelicAutomation<Settings>
    {
    public:
        Fps fpsLimit = 0;
        static const Fps defaultFpsLimit;

        bool verticalSync = false;
    };
}

namespace Arca
{
    template<>
    struct Traits<::Atmos::Time::Settings>
    {
        static const ObjectType objectType = ObjectType::Relic;
        static inline const TypeName typeName = "TimeSettings";
        static const Locality locality = Locality::Global;
    };
}

namespace Inscription
{
    template<>
    class Scribe<::Atmos::Time::Settings, BinaryArchive> final :
        public ArcaNullScribe<::Atmos::Time::Settings, BinaryArchive>
    {};
}