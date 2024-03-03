#include "RenderCurator.h"

#include "MainSurface.h"

#include "StartStopwatch.h"
#include "CalculateStopwatch.h"

#include "DebugStatistics.h"

namespace Atmos::Render
{
    RenderCurator::RenderCurator(Init init) :
        Curator(init), debugStatistics(init.owner)
    {}

    void RenderCurator::Work(Stage& stage)
    {
        Owner().Do<Time::StartStopwatch>(debugStatistics->profilers.render.ID());

        Data<MainSurface>()->RenderStaged();

        Owner().Do<Time::CalculateStopwatch>(debugStatistics->profilers.render.ID());
    }
}
