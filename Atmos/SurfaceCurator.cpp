#include "SurfaceCurator.h"
#include "SurfaceCore.h"

#include "MainSurface.h"
#include "GraphicsManager.h"

namespace Atmos::Render
{
    SurfaceCurator::SurfaceCurator(Init init) : Curator(init)
    {}

    void SurfaceCurator::Handle(const ChangeSurfaceBackgroundColor& command)
    {
        auto surfaceCore = MutablePointer().Of<SurfaceCore>(command.id);

        surfaceCore->backgroundColor = command.color;
    }

    void SurfaceCurator::Handle(const CreateMainSurfaceData& command)
    {
        auto surfaceCore = MutablePointer().Of<SurfaceCore>(Arca::Index<MainSurface>(Owner()).ID());

        auto data = Arca::Postulate<GraphicsManager*>(Owner()).Get()->CreateMainSurfaceData(command.window);
        surfaceCore->data = std::move(data);
    }
}
