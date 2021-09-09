#include "RenderCurator.h"

#include "RenderCore.h"
#include "ViewSlice.h"
#include "ColorChanged.h"

#include "Camera.h"

#include "CreateStopwatch.h"
#include "DiagnosticsStatistics.h"

namespace Atmos::Render
{
    Curator::Curator(Init init) : Arca::Curator(init), mainSurface(init.owner.Find<MainSurface>())
    {}

    void Curator::Handle(const Work&)
    {
        auto stopwatch = Time::CreateRealStopwatch();

        const auto camera = Owner().Find<Camera>();
        const auto mapPosition = Spatial::Point2D
        {
            Spatial::Point2D::Value(camera->Position().x),
            Spatial::Point2D::Value(camera->Position().y)
        };

        const auto mutableMainPointer = MutablePointer().Of(mainSurface);
        mutableMainPointer->DrawFrame(mapPosition);

        const auto idleDuration = std::chrono::duration_cast<std::chrono::duration<double>>(stopwatch.Calculate());
        MutablePointer().Of<Diagnostics::Statistics>()->renderTime = idleDuration.count();
    }

    void Curator::Handle(const ChangeColor& command)
    {
        auto renderCore = MutablePointer().Of<RenderCore>(command.id);
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
