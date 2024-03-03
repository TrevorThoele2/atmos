#include "MainSurface.h"

namespace Atmos::Render
{
    MainSurface::MainSurface(Init init, DataPtr&& data) : Surface(init, std::move(data))
    {}

    void MainSurface::RenderStagedImpl(GraphicsManager& graphicsManager)
    {
        graphicsManager.RenderStaged(Core()->data->Size(), Core()->backgroundColor);
    }
}