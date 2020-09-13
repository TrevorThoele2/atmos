#pragma once

#include <Arca/ClosedTypedRelic.h>

#include "FramesPerSecond.h"

namespace Atmos::Frame
{
    class Settings final : public Arca::ClosedTypedRelic<Settings>
    {
    public:
        FramesPerSecond framesPerSecondLimit = 0;
        static inline const FramesPerSecond defaultFramesPerSecondLimit = 60;

        using ClosedTypedRelic::ClosedTypedRelic;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Frame::Settings>
    {
        static const ObjectType objectType = ObjectType::Relic;
        static inline const TypeName typeName = "Atmos::Frame::Settings";
        static const Locality locality = Locality::Global;
    };
}

namespace Inscription
{
    template<class Archive>
    struct ScribeTraits<Atmos::Frame::Settings, Archive> final
    {
        using Category = ArcaNullScribeCategory<Atmos::Frame::Settings>;
    };
}