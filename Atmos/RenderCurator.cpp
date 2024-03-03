#include "RenderCurator.h"

namespace Atmos::Render
{
    Curator::Curator(Init init) :
        Arca::Curator(init), mainSurface(init.owner),
        debugRenderProfiler(
            [](Debug::Statistics& statistics) -> Arca::Index<Time::RealStopwatch>&
            {
                return statistics.profilers.render;
            },
            MutablePointer())
    {}

    void Curator::Work()
    {
        debugRenderProfiler.Start();

        auto mainSurfaceData = MutablePointer().Of(mainSurface);
        mainSurfaceData->RenderStaged();

        debugRenderProfiler.Calculate();
    }
}
