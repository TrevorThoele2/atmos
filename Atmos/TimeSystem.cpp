
#include "TimeSystem.h"

#include "ObjectManager.h"
#include "FpsSystem.h"

namespace Atmos
{
    TimeSystem::TimeSystem(ObjectManager& manager) : ObjectSystem(manager)
    {}

    void TimeSystem::OnFrameEnd()
    {
        auto fpsSystem = Manager()->FindSystem<FpsSystem>();
        lastElapsed = (fpsSystem->GetFrameEnd() - fpsSystem->GetFrameStart());
        totalElapsed += lastElapsed;
    }

    TimeValue TimeSystem::GetTotalElapsed() const
    {
        return totalElapsed;
    }

    TimeValue TimeSystem::GetLastElapsed() const
    {
        return lastElapsed;
    }

    TimeValue TimeSystem::CurrentTime() const
    {
        auto nanosecondsTime = clock.now().time_since_epoch().count();
        return TimeValue(nanosecondsTime, TimeValueEpoch::NANOSECONDS).ConvertValue(TimeValueEpoch::SECONDS);
    }
}

namespace Inscription
{
    void Scribe<::Atmos::TimeSystem, BinaryArchive>::Scriven(ObjectT& object, ArchiveT& archive)
    {
        BaseScriven<::Atmos::ObjectSystem>(object, archive);
    }
}