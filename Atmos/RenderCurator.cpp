#include "RenderCurator.h"

#include "RenderCore.h"
#include "ViewSlice.h"
#include "ColorChanged.h"

#include "CreateStopwatch.h"
#include "DiagnosticsStatistics.h"

namespace Atmos::Render
{
    Curator::Curator(Init init) : Arca::Curator(init), mainSurface(init.owner)
    {}

    void Curator::Handle(const Work&)
    {
        auto stopwatch = Time::CreateRealStopwatch();

        const auto mutableMainPointer = MutablePointer().Of(mainSurface);
        mutableMainPointer->DrawFrame();

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
