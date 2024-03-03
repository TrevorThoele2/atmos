#include "SurfaceCurator.h"

#include "MainSurface.h"

namespace Atmos::Render
{
    SurfaceCurator::SurfaceCurator(Init init) : Curator(init)
    {}

    void SurfaceCurator::Handle(const ChangeMainSurfaceBackgroundColor& command)
    {
        auto surface = MutablePointer().Of<SurfaceCore>(command.id);
        surface->backgroundColor = command.color;
    }
}
