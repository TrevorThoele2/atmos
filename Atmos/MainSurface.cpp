#include "MainSurface.h"

namespace Atmos::Render
{
    MainSurface::MainSurface(Arca::RelicInit init, GraphicsManager& manager, void* window) :
        Surface(init, manager.CreateMainSurfaceResource(window, init.owner))
    {}
}