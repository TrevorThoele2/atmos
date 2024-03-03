#pragma once

#include "ArcaRelicIncludes.h"

#include "FramesPerSecond.h"

namespace Atmos::Frame
{
    class Settings final
    {
    public:
        FramesPerSecond framesPerSecondLimit = defaultFramesPerSecondLimit;
        static inline const FramesPerSecond defaultFramesPerSecondLimit = 60;

        Settings() = default;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Frame::Settings>
    {
        static constexpr ObjectType objectType = ObjectType::Relic;
        static const inline TypeName typeName = "Atmos::Frame::Settings";
        static constexpr Locality locality = Locality::Global;
    };
}

namespace Inscription
{
    template<class Format>
    struct ScribeTraits<Atmos::Frame::Settings, Format> final
    {
        using Category = ArcaNullScribeCategory<Atmos::Frame::Settings>;
    };
}