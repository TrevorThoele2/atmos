#include "RealStopwatch.h"

#include <chrono>

#include "StartStopwatch.h"
#include "CalculateStopwatch.h"

namespace Atmos::Time
{
    RealStopwatch::RealStopwatch(Init init) :
        OpenTypedRelic(init), core(init.Create<Core>([this]() { return CurrentTime(); }))
    {}

    Value<> RealStopwatch::Start() const
    {
        return Owner().Do<StartStopwatch>(ID());
    }

    bool RealStopwatch::IsStarted() const
    {
        return core->IsStarted();
    }

    Duration<> RealStopwatch::Elapsed() const
    {
        return Owner().Do<CalculateStopwatch>(ID());
    }

    Value<> RealStopwatch::CurrentTime() const
    {
        return std::chrono::high_resolution_clock::now();
    }
}

namespace Inscription
{
    void Scribe<::Atmos::Time::RealStopwatch, BinaryArchive>::ScrivenImplementation(
        ObjectT& object, ArchiveT& archive)
    {}
}