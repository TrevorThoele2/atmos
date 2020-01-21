#include "TimeCurator.h"

#include <Arca/Reliquary.h>

namespace Atmos::Time
{
    void TimeCurator::StartFrame()
    {
        timeInformation->frameStartTime = timeInformation->stopwatch.Start();
    }

    bool TimeCurator::DoIdle()
    {
        if (!IsFrameDone())
            return true;
        else
        {
            FrameDone();
            return false;
        }
    }

    void TimeCurator::InitializeImplementation()
    {
        timeInformation = Arca::GlobalIndex<Information>(Owner());
        timeSettings = Arca::GlobalIndex<Settings>(Owner());

        fpsTimer = FpsStopwatch{ Owner().Create<FpsStopwatch::SourceT>(Value(Value::Number(1, 0))) };

        StartFrame();
    }

    bool TimeCurator::IsFrameDone() const
    {
        if (timeSettings->fpsLimit == 0 || timeSettings->verticalSync)
            return true;

        return timeInformation->stopwatch.QueryElapsed().Get() >= Value::Number(timeSettings->fpsLimit);
    }

    void TimeCurator::FrameDone()
    {
        static unsigned int count = 0;

        if (fpsTimer.QueryElapsed().Get() >= Value::Number(timeSettings->fpsLimit))
        {
            fpsTimer.Start();

            timeInformation->fps = count;
            count = 0;
        }

        ++count;

        timeInformation->frameEndTime = timeInformation->stopwatch.Checkpoint();
        timeInformation->lastFrameElapsed = timeInformation->frameEndTime - timeInformation->frameStartTime;
        timeInformation->totalElapsed += timeInformation->lastFrameElapsed;
    }
}

namespace Inscription
{
    void Scribe<::Atmos::Time::TimeCurator, BinaryArchive>::ScrivenImplementation(ObjectT& object, ArchiveT& archive)
    {
        if (archive.IsOutput())
        {
            auto fpsTimerId = object.fpsTimer.Source().ID();
            archive(fpsTimerId);
        }
        else
        {
            Arca::RelicID fpsTimerId;
            archive(fpsTimerId);

            const auto reliquary = archive.UserContext<Arca::Reliquary>();
            object.fpsTimer.Source(Arca::RelicIndex<ObjectT::FpsStopwatch::SourceT>(fpsTimerId, object.Owner()));
        }
    }
}