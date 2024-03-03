#include "SurfaceCurator.h"
#include "SurfaceCore.h"

#include "MainSurface.h"

namespace Atmos::Render
{
    SurfaceCurator::SurfaceCurator(Init init) : Curator(init)
    {}

    void SurfaceCurator::Handle(const ChangeSurfaceBackgroundColor& command)
    {
        auto surfaceCore = MutablePointer().Of<SurfaceCore>(command.id);

        surfaceCore->backgroundColor = command.color;
    }
}
