#include "FrameStopwatch.h"

#include <Arca/Reliquary.h>

namespace Atmos::Time
{
    bool FrameStopwatch::operator==(const FrameStopwatch& arg) const
    {
        return Stopwatch::operator==(arg);
    }

    bool FrameStopwatch::operator!=(const FrameStopwatch& arg) const
    {
        return !(*this == arg);
    }

    Value FrameStopwatch::CurrentTime() const
    {
        return timeInformation->totalElapsed;
    }

    void FrameStopwatch::PostConstruct()
    {
        timeInformation = Arca::GlobalIndex<Information>(Owner());
    }
}

namespace Inscription
{
    void Scribe<::Atmos::Time::FrameStopwatch, BinaryArchive>::ScrivenImplementation(
        ObjectT& object, ArchiveT& archive)
    {
        if (archive.IsInput())
            object.timeInformation = Arca::GlobalIndex<Atmos::Time::Information>(object.Owner());

        BaseScriven<Atmos::Time::Stopwatch>(object, archive);
    }
}