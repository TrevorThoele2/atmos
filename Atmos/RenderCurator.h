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
    class GraphicsCurator;

    class RenderCurator final : public Arca::Curator
    {
    public:
        enum class Flag
        {
            Draw
        };

        Flags<Flag> flags;
    protected:
        void InitializeImplementation() override;
        void WorkImplementation(Stage& stage) override;
    private:
        Arca::ComputedPtr<GraphicsManager*> graphics;
    private:
        Arca::GlobalPtr<Debug::Statistics> debugStatistics;
    };
}

namespace Inscription
{
    template<>
    class Scribe<::Atmos::Render::RenderCurator, BinaryArchive> final :
        public ArcaNullScribe<::Atmos::Render::RenderCurator, BinaryArchive>
    {};
}