#pragma once

#include <Arca/Curator.h>

#include "Flags.h"

#include "GraphicsManager.h"

namespace Atmos::Debug
{
    class Statistics;
}

namespace Atmos::Render
{
    class RenderCurator final : public Arca::Curator
    {
    public:
        explicit RenderCurator(Init init);

        void Work(Stage& stage);
    private:
        Arca::GlobalIndex<Debug::Statistics> debugStatistics;
    };
}

namespace Inscription
{
    template<>
    class Scribe<::Atmos::Render::RenderCurator, BinaryArchive> final :
        public ArcaNullScribe<::Atmos::Render::RenderCurator, BinaryArchive>
    {};
}