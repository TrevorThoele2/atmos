#include "RenderCurator.h"

#include "ImageCore.h"
#include "Line.h"

namespace Atmos::Render
{
    Curator::Curator(Init init) :
        Arca::Curator(init), mainSurface(init.owner),
        debugRenderProfiler(
            [](Debug::Statistics& statistics) -> Time::Stopwatch&
            {
                return statistics.profilers.render;
            },
            MutablePointer())
    {}

    void Curator::Handle(const Work&)
    {
        debugRenderProfiler.Start();

        const auto mutableMainPointer = MutablePointer().Of(mainSurface);
        mutableMainPointer->DrawFrame();

        debugRenderProfiler.Calculate();
    }

    void Curator::Handle(const ChangeColor& command)
    {
        const auto execute = [this, command](auto index)
        {
            auto mutableObject = MutablePointer().Of(index);
            mutableObject->color = command.to;
        };

        const auto image = Arca::Index<ImageCore>(command.id, Owner());
        if (image)
        {
            execute(image);
            return;
        }

        const auto line = Arca::Index<Line>(command.id, Owner());
        if (line)
        {
            execute(line);
            return;
        }
    }
}
