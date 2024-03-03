#include "SurfaceCurator.h"
#include "SurfaceCore.h"

namespace Atmos::Render
{
    void SurfaceCurator::Handle(const ChangeSurfaceBackgroundColor& command)
    {
        auto surfaceCore = MutablePointer().Of<SurfaceCore>(command.id);

        surfaceCore->backgroundColor = command.color;
    }
}
