#include "DebugStatistics.h"

#include "StopwatchStatistics.h"

namespace Atmos::Debug
{
    Statistics::Statistics(Init init) :
        ClosedTypedRelic(init),
        profilers(init.owner)
    {}

    Statistics::Profilers::Profilers(Arca::Reliquary& owner) :
        input(CreateProfiler(owner)),
        logic(CreateProfiler(owner)),
        render(CreateProfiler(owner)),
        idle(CreateProfiler(owner)),
        misc1(CreateProfiler(owner)),
        misc2(CreateProfiler(owner)),
        misc3(CreateProfiler(owner))
    {}

    auto Statistics::Profilers::CreateProfiler(Arca::Reliquary& owner) -> Profiler
    {
        auto created = owner.Do<Arca::Create<Profiler::ValueT>>();
        created->Create<Time::StopwatchStatistics>();
        return created;
    }
}

namespace Inscription
{
    void Scribe<Atmos::Debug::Statistics, BinaryArchive>::ScrivenImplementation(ObjectT& object, ArchiveT& archive)
    {
        archive(object.profilers.input);
        archive(object.profilers.logic);
        archive(object.profilers.render);
        archive(object.profilers.idle);
        archive(object.profilers.misc1);
        archive(object.profilers.misc2);
        archive(object.profilers.misc3);
    }
}