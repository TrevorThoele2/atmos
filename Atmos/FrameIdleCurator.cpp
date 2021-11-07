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
        auto idleFor = IdleFor();
        while (idleFor)
        {
            windowInformation->window->Suspend(std::chrono::duration_cast<Time::Milliseconds>(*idleFor));
            idleFor = IdleFor();
        }
    }

    std::optional<Time::Nanoseconds> IdleCurator::IdleFor()
    {
        const auto settings = Owner().Find<Settings>();
        const auto graphicsSettings = Owner().Find<Render::GraphicsSettings>();

        if (settings->framesPerSecondLimit == 0 || graphicsSettings->verticalSync)
            return {};

        const auto frameInformation = Owner().Find<Information>();
        const auto elapsed = frameInformation->profilers.frame.Elapsed();
        const auto expectedTime = std::chrono::duration_cast<Time::Nanoseconds>(Time::Seconds(1)) / settings->framesPerSecondLimit;

        return elapsed >= expectedTime
            ? std::optional<Time::Nanoseconds>{}
            : std::optional<Time::Nanoseconds>{ expectedTime - elapsed };
    }
}