#pragma once

#include <Arca/Curator.h>

#include "Flags.h"

#include "GraphicsManagerProvider.h"

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
        GraphicsManagerProvider* graphics = nullptr;
    private:
        Debug::Statistics* debugStatistics = nullptr;
    };
}

namespace Inscription
{
    template<>
    class Scribe<::Atmos::Render::RenderCurator, BinaryArchive> final :
        public ArcaNullScribe<::Atmos::Render::RenderCurator, BinaryArchive>
    {};
}