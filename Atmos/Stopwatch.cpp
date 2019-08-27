#include "Stopwatch.h"

namespace Atmos::Time
{
    Stopwatch::~Stopwatch() = default;

    bool Stopwatch::operator==(const Stopwatch& arg) const
    {
        return start == arg.start;
    }

    bool Stopwatch::operator!=(const Stopwatch& arg) const
    {
        return !(*this == arg);
    }

    Value Stopwatch::Start()
    {
        start = CurrentTime();
        return start;
    }

    Value Stopwatch::Elapsed() const
    {
        return CurrentTime() - start;
    }
}

namespace Inscription
{
    void Scribe<::Atmos::Time::Stopwatch, BinaryArchive>::ScrivenImplementation(
        ObjectT& object, ArchiveT& archive)
    {
        if (archive.IsOutput())
        {
            auto timeStamp = object.CurrentTime() - object.start;
            archive(timeStamp);
        }
        else // INPUT
        {
            ::Atmos::Time::Value timeStamp;
            archive(timeStamp);
            object.start = object.CurrentTime() - timeStamp;
        }
    }
}