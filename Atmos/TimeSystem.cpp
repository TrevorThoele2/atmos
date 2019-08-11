#include "TimeSystem.h"

#include "ObjectManager.h"
#include "FpsSystem.h"

namespace Atmos::Time
{
    TimeSystem::TimeSystem(ObjectManager& manager) : ObjectSystem(manager)
    {}

    void TimeSystem::OnFrameEnd()
    {
        auto fpsSystem = Manager()->FindSystem<FpsSystem>();
        lastElapsed = (fpsSystem->GetFrameEnd() - fpsSystem->GetFrameStart());
        totalElapsed += lastElapsed;
    }

    Value TimeSystem::GetTotalElapsed() const
    {
        return totalElapsed;
    }

    Value TimeSystem::GetLastElapsed() const
    {
        return lastElapsed;
    }

    Value TimeSystem::CurrentTime() const
    {
        auto nanosecondsTime = clock.now().time_since_epoch().count();
        return Value(nanosecondsTime, Epoch::NANOSECONDS).GetAs(Epoch::SECONDS);
    }
}

namespace Inscription
{
    void Scribe<::Atmos::Time::TimeSystem, BinaryArchive>::ScrivenImplementation(ObjectT& object, ArchiveT& archive)
    {
        BaseScriven<::Atmos::ObjectSystem>(object, archive);
    }
}