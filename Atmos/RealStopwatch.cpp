#include "RealStopwatch.h"

#include <chrono>

#include "StartStopwatch.h"
#include "CalculateStopwatch.h"

namespace Atmos::Time
{
    RealStopwatch::RealStopwatch(Init init) :
        OpenTypedRelic(init)
    {
        core = Create<Core>(
            [this]()
            {
                return CurrentTime();
            });
    }

    Value RealStopwatch::Start() const
    {
        return Owner().Do<StartStopwatch>(ID());
    }

    Value RealStopwatch::Elapsed() const
    {
        return Owner().Do<CalculateStopwatch>(ID());
    }

    Value RealStopwatch::CurrentTime() const
    {
        const auto nanosecondsTime = std::chrono::high_resolution_clock::now().time_since_epoch().count();
        return Value(Value(nanosecondsTime, Epoch::Nanoseconds).GetAs(Epoch::Seconds));
    }
}

namespace Inscription
{
    void Scribe<::Atmos::Time::RealStopwatch, BinaryArchive>::ScrivenImplementation(
        ObjectT& object, ArchiveT& archive)
    {}
}