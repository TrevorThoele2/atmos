#pragma once

#include <Arca/ClosedTypedRelic.h>

#include "TimeValue.h"
#include "Fps.h"

namespace Atmos::Time
{
    class Settings final : public Arca::ClosedTypedRelic<Settings>
    {
    public:
        Fps fpsLimit = 0;
        static const Fps defaultFpsLimit;

        bool verticalSync = false;

        explicit Settings(Init init) : ClosedTypedRelic(init)
        {}
        Settings(const Settings& arg) = default;
    };
}

namespace Arca
{
    template<>
    struct Traits<::Atmos::Time::Settings>
    {
        static const ObjectType objectType = ObjectType::Relic;
        static inline const TypeName typeName = "Atmos::Time::Settings";
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