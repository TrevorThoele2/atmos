#include "AncillarySurface.h"

namespace Atmos::Render
{
    AncillarySurface::AncillarySurface(Init init, DataPtr&& data) : Surface(init, std::move(data))
    {}

    void AncillarySurface::RenderStagedImpl(GraphicsManager& graphicsManager)
    {
        graphicsManager.RenderStaged(*Data(), Core()->backgroundColor);
    }
}