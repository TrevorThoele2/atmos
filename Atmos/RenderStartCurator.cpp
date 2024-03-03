#include "RenderStartCurator.h"

#include "CreateStopwatch.h"

namespace Atmos::Render
{
    StartCurator::StartCurator(Init init) : Curator(init)
    {}

    void StartCurator::Handle(const Work&)
    {
        auto frameInformation = MutablePointer().Of<Frame::Information>();

        frameInformation->profilers.render = Time::CreateRealStopwatch();
    }
}
