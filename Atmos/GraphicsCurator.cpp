#include "GraphicsCurator.h"

#include <Arca/Reliquary.h>

#include "ReconstructGraphics.h"
#include "WindowDimensionsChanged.h"

namespace Atmos::Render
{
    void GraphicsCurator::InitializeImplementation()
    {
        manager = Arca::ComputedPtr<GraphicsManager*>(Owner());

        Owner().ExecuteOn<ReconstructGraphics>([this](const ReconstructGraphics&)
            {
                (*manager)->ReconstructAll();
            });

        Owner().ExecuteOn<Window::DimensionsChanged>([this](const Window::DimensionsChanged& signal)
            {
                (*manager)->SetMainDimensions(signal.dimensions);
            });
    }
}

namespace Arca
{
    const TypeName Traits<Atmos::Render::GraphicsCurator>::typeName = "GraphicsCurator";
}