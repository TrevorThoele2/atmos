#include "FrameStopwatch.h"

#include "StartStopwatch.h"
#include "CalculateStopwatch.h"

namespace Atmos::Time
{
    FrameStopwatch::FrameStopwatch(Init init) :
        OpenTypedRelic(init),
        core(init.Create<Core>([this]() { return CurrentTime(); })),
        timeInformation(init.owner)
    {}

    Value<> FrameStopwatch::Start() const
    {
        return Owner().Do<StartStopwatch>(ID());
    }

    bool FrameStopwatch::IsStarted() const
    {
        return core->IsStarted();
    }

    Duration<> FrameStopwatch::Elapsed() const
    {
        return Owner().Do<CalculateStopwatch>(ID());
    }

    Value<> FrameStopwatch::CurrentTime() const
    {
        return Time::Value<>() + timeInformation->totalElapsed;
    }
}

namespace Inscription
{
    void Scribe<::Atmos::Time::FrameStopwatch, BinaryArchive>::ScrivenImplementation(
        ObjectT& object, ArchiveT& archive)
    {}
}