#pragma once

#include <Arca/Curator.h>
#include "TimeInformation.h"

#include "DebugProfiler.h"

namespace Atmos
{
    class FrameStartCurator final : public Arca::Curator
    {
    public:
        explicit FrameStartCurator(Init init);

        void Work();
    private:
        Arca::Index<Time::Information> timeInformation;
    private:
        Debug::Profiler debugIdleProfiler;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::FrameStartCurator>
    {
        static const ObjectType objectType = ObjectType::Curator;
        static inline const TypeName typeName = "FrameStartCurator";
    };
}

namespace Inscription
{
    template<>
    class Scribe<Atmos::FrameStartCurator, BinaryArchive> final
        : public ArcaNullScribe<Atmos::FrameStartCurator, BinaryArchive>
    {};
}