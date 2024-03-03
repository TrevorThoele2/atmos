
#include "TimeSystem.h"

#include "ObjectManager.h"
#include "FpsSystem.h"

namespace Atmos
{
    TimeSystem::TimeSystem(ObjectManager& manager) : ObjectSystem(manager)
    {}

    TimeSystem::TimeSystem(const ::Inscription::Table<TimeSystem>& table) : INSCRIPTION_TABLE_GET_BASE(ObjectSystem)
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
    INSCRIPTION_INSCRIPTER_DEFINE_TABLE(::Atmos::TimeSystem)
    {
        INSCRIPTION_INSCRIPTER_CREATE_TABLE;

        INSCRIPTION_TABLE_ADD_BASE(::Atmos::ObjectSystem);

        INSCRIPTION_INSCRIPTER_RETURN_TABLE;
    }

    INSCRIPTION_DEFINE_SIMPLE_CLASS_NAME_RESOLVER(::Atmos::TimeSystem, "TimeSystem");
}