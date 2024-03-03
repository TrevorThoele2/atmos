#pragma once

#include <Arca/Curator.h>

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
        unsigned int frameCount = 0;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Frame::EndCurator>
    {
        static const ObjectType objectType = ObjectType::Curator;
        static TypeName TypeName() { return "Atmos::Frame::EndCurator"; }
        using HandledCommands = HandledCommands<Atmos::Work>;
    };
}

namespace Inscription
{
    template<class Archive>
    struct ScribeTraits<Atmos::Frame::EndCurator, Archive> final
    {
        using Category = ArcaNullScribeCategory<Atmos::Frame::EndCurator>;
    };
}