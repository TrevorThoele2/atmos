#include "RenderCurator.h"

#include "RenderCore.h"
#include "ViewSlice.h"
#include "ColorChanged.h"

#include "MainSurface.h"
#include "Camera.h"

namespace Atmos::Render
{
    Curator::Curator(Init init, GraphicsManager& graphicsManager) : Arca::Curator(init), graphicsManager(&graphicsManager)
    {}

    void Curator::Handle(const Work&)
    {
        const auto camera = Owner().Find<Camera>();
        const auto mapPosition = Spatial::Point2D
        {
            Spatial::Point2D::Value(camera->Position().x),
            Spatial::Point2D::Value(camera->Position().y)
        };

        const auto mainSurface = Owner().Find<MainSurface>();
        const auto backgroundColor = mainSurface->core->backgroundColor;

        graphicsManager->DrawFrame(*mainSurface->Resource(), mapPosition, backgroundColor, MutablePointer().Of<Diagnostics::Statistics>()->misc);
    }

    void Curator::Handle(const ChangeColor& command)
    {
        const auto renderCore = MutablePointer().Of<RenderCore>(command.id);
        if (renderCore)
        {
            const auto previous = renderCore->color;
            renderCore->color = command.to;
            Owner().Raise(ColorChanged{ command.id, previous });
        }
    }

    void Curator::Handle(const ChangeMaterialAsset& command)
    {
        const auto renderCore = MutablePointer().Of<RenderCore>(command.id);
        if (renderCore)
            renderCore->material = command.to;
    }

    void Curator::Handle(const ChangeViewSlice& command)
    {
        const auto viewSlice = MutablePointer().Of<ViewSlice>(command.id);
        if (viewSlice)
            viewSlice->box = command.to;
    }
}
