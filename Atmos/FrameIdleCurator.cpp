#include "FrameIdleCurator.h"

#include "GraphicsSettings.h"
#include "FrameSettings.h"
#include "FrameInformation.h"

#include "WindowProvider.h"

namespace Atmos::Frame
{
    IdleCurator::IdleCurator(Init init) : Curator(init)
    {}

    void IdleCurator::Work()
    {
        while (!IsDone())
            Window::window->Suspend(Time::Duration<>(0));
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