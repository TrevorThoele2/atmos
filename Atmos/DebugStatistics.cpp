#include "DebugStatistics.h"

namespace Atmos::Debug
{
    void Statistics::Initialize()
    {
        const auto setSource = [this](Profiler& profiler)
        {
            profiler.Source(Owner().Create<Time::RealStopwatch>());
        };

        setSource(profilers.input);
        setSource(profilers.logic);
        setSource(profilers.render);
        setSource(profilers.frameTime);
        setSource(profilers.idle);
        setSource(profilers.misc1);
        setSource(profilers.misc2);
        setSource(profilers.misc3);
    }
}

namespace Arca
{
    const TypeName Traits<::Atmos::Debug::Statistics>::typeName = "DebugStatistics";
}

namespace Inscription
{
    void Scribe<Atmos::Debug::Statistics, BinaryArchive>::ScrivenImplementation(ObjectT& object, ArchiveT& archive)
    {
        if (archive.IsOutput())
        {
            const auto saveProfiler = [&archive](ObjectT::Profiler& profiler)
            {
                auto id = profiler.Source().ID();
                archive(id);
            };

            saveProfiler(object.profilers.input);
            saveProfiler(object.profilers.logic);
            saveProfiler(object.profilers.render);
            saveProfiler(object.profilers.frameTime);
            saveProfiler(object.profilers.idle);
            saveProfiler(object.profilers.misc1);
            saveProfiler(object.profilers.misc2);
            saveProfiler(object.profilers.misc3);
        }
        else
        {
            const auto loadProfiler = [&archive](ObjectT::Profiler& profiler)
            {
                Arca::RelicID id;
                archive(id);

                const auto reliquary = archive.UserContext<Arca::Reliquary>();
                profiler.Source(Arca::LocalPtr<ObjectT::Profiler::SourceT>(id, *reliquary));
            };

            loadProfiler(object.profilers.input);
            loadProfiler(object.profilers.logic);
            loadProfiler(object.profilers.render);
            loadProfiler(object.profilers.frameTime);
            loadProfiler(object.profilers.idle);
            loadProfiler(object.profilers.misc1);
            loadProfiler(object.profilers.misc2);
            loadProfiler(object.profilers.misc3);
        }
    }
}