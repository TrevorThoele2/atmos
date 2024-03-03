#include "RenderCurator.h"

#include <Arca/Reliquary.h>
#include "DebugStatistics.h"
#include "GraphicsCurator.h"

namespace Atmos::Render
{
    void RenderCurator::InitializeImplementation()
    {
        graphics = Arca::ComputedPtr<GraphicsManager*>(Owner());

        debugStatistics = Arca::GlobalPtr<Debug::Statistics>(Owner());
    }

    void RenderCurator::WorkImplementation(Stage& stage)
    {
        debugStatistics->profilers.render.Start();

        auto& useGraphics = **graphics;

        if (!useGraphics.Start())
        {
            debugStatistics->profilers.render.Calculate();
            stage.Abort();
            return;
        }

        useGraphics.StartObjects();
        useGraphics.ClearTarget(
            Flags<GraphicsManager::Target>({ GraphicsManager::Target::Main }),
            Color(0, 255, 0, 0));

        if (flags.Get(Flag::Draw))
        {

        }

        useGraphics.Flush();

        useGraphics.StopObjects();

        useGraphics.Stop();
        useGraphics.Present();

        debugStatistics->profilers.render.Calculate();
    }
}