#include "FrameIdleCurator.h"

#include "GraphicsSettings.h"
#include "FrameSettings.h"
#include "FrameInformation.h"

namespace Atmos::Frame
{
    IdleCurator::IdleCurator(Init init) : Curator(init), windowInformation(init.owner)
    {}

    void IdleCurator::Handle(const Work&)
    {
        while (!IsDone())
            windowInformation->window->Suspend(Time::Duration<>(0));
    }

    bool IdleCurator::IsDone()
    {
        const auto settings = Arca::Index<Settings>(Owner());
        const auto graphicsSettings = Arca::Index<Render::GraphicsSettings>(Owner());

        const auto frameInformation = Arca::Index<Information>(Owner());

        if (settings->framesPerSecondLimit == 0 || graphicsSettings->verticalSync)
            return true;

        return frameInformation->stopwatch.Elapsed() >= Time::Milliseconds(settings->framesPerSecondLimit);
    }
}