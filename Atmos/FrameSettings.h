#pragma once

#include "ArcaRelicIncludes.h"

#include "FramesPerSecond.h"

namespace Atmos::Frame
{
    class Settings final
    {
    public:
        FramesPerSecond framesPerSecondLimit = 0;
        static inline const FramesPerSecond defaultFramesPerSecondLimit = 60;

        Settings() = default;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Frame::Settings>
    {
        static const ObjectType objectType = ObjectType::Relic;
        static TypeName TypeName() { return "Atmos::Frame::Settings"; }
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