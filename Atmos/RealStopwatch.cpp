#include "RealStopwatch.h"

#include <chrono>

namespace Atmos::Time
{
    bool RealStopwatch::operator==(const RealStopwatch& arg) const
    {
        return Stopwatch::operator==(arg);
    }

    Value RealStopwatch::CurrentTime() const
    {
        const auto nanosecondsTime = std::chrono::high_resolution_clock::now().time_since_epoch().count();
        return Value(Value(nanosecondsTime, Epoch::Nanoseconds).GetAs(Epoch::Seconds));
    }
}

namespace Arca
{
    const TypeName Traits<::Atmos::Time::RealStopwatch>::typeName = "RealStopwatch";
}

namespace Inscription
{
    void Scribe<::Atmos::Time::RealStopwatch, BinaryArchive>::ScrivenImplementation(
        ObjectT& object, ArchiveT& archive)
    {
        BaseScriven<Atmos::Time::Stopwatch>(object, archive);
    }
}