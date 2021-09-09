#include "FrameIdleCurator.h"

#include "GraphicsSettings.h"
#include "FrameSettings.h"
#include "FrameInformation.h"

namespace Atmos::Frame
{
    IdleCurator::IdleCurator(Init init) : Curator(init), windowInformation(init.owner.Find<Window::Information>())
    {}

    void IdleCurator::Handle(const Work&)
    {
        while (!IsDone())
            windowInformation->window->Suspend(Time::Duration<>(0));
    }

    bool IdleCurator::IsDone()
    {
        const auto settings = Owner().Find<Settings>();
        const auto graphicsSettings = Owner().Find<Render::GraphicsSettings>();

        const auto frameInformation = Owner().Find<Information>();

        if (settings->framesPerSecondLimit == 0 || graphicsSettings->verticalSync)
            return true;

        return frameInformation->profilers.frame.Elapsed() >= Time::Milliseconds(settings->framesPerSecondLimit);
    }
}