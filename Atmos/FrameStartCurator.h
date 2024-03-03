#pragma once

#include <Arca/Curator.h>

#include "DebugProfiler.h"

namespace Atmos::Frame
{
    class StartCurator final : public Arca::Curator
    {
    public:
        explicit StartCurator(Init init);

        void Work();
    private:
        Debug::Profiler debugIdleProfiler;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Frame::StartCurator>
    {
        static const ObjectType objectType = ObjectType::Curator;
        static inline const TypeName typeName = "Atmos::Frame::StartCurator";
    };
}

namespace Inscription
{
    template<class Archive>
    struct ScribeTraits<Atmos::Frame::StartCurator, Archive> final
    {
        using Category = ArcaNullScribeCategory<Atmos::Frame::StartCurator>;
    };
}