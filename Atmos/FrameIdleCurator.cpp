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

        if (settings->framesPerSecondLimit == 0 || graphicsSettings->verticalSync)
            return true;

        const auto frameInformation = Owner().Find<Information>();
        const auto elapsed = frameInformation->profilers.frame.Elapsed();
        const auto expectedTime = std::chrono::duration_cast<Time::Nanoseconds>(Time::Seconds(1)) / settings->framesPerSecondLimit;

        return elapsed >= expectedTime;
    }
}