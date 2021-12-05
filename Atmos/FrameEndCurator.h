#pragma once

#include <Arca/Curator.h>

#include "FrameInformation.h"
#include "FramesPerSecond.h"
#include "Stopwatch.h"

#include "Work.h"

namespace Atmos::Frame
{
    class EndCurator final : public Arca::Curator
    {
    public:
        explicit EndCurator(Init init);

        void Handle(const Work& command);
    private:
        Time::Stopwatch framesPerSecondStopwatch;
        FramesPerSecond frameCount = 0;

        void CalculateFPS(Information& information);
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Frame::EndCurator>
    {
        static constexpr ObjectType objectType = ObjectType::Curator;
        static const inline TypeName typeName = "Atmos::Frame::EndCurator";
        using HandledCommands = HandledCommands<Atmos::Work>;
    };
}

namespace Inscription
{
    template<class Format>
    struct ScribeTraits<Atmos::Frame::EndCurator, Format> final
    {
        using Category = ArcaNullScribeCategory<Atmos::Frame::EndCurator>;
    };
}