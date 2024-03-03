
#include "FpsSystem.h"

#include "ObjectManager.h"
#include "InitializationFileSystem.h"
#include "TimeSystem.h"

#include "FrameStopwatch.h"

namespace Atmos
{
    FpsSystem::FpsSystem(ObjectManager& manager) : ObjectSystem(manager), stopwatch(manager)
    {}

    void FpsSystem::SetFpsLimit(Fps fps)
    {
        currentFpsLimit = fps;
        if (currentFpsLimit == 0)
            stopwatch.SetGoal(TimeValue::Value(0));
        else
            stopwatch.SetGoal(TimeValue::Value(1) / TimeValue::Value(currentFpsLimit));
    }

    FpsSystem::Fps FpsSystem::GetFpsLimit()
    {
        return currentFpsLimit;
    }

    void FpsSystem::SetVSync(bool set)
    {
        vSync = set;
    }

    bool FpsSystem::GetVSync()
    {
        return vSync;
    }

    void FpsSystem::StartFrame()
    {
        start = stopwatch.Start();
    }

    bool FpsSystem::DoIdle()
    {
        if (!IsFrameDone())
            return true;
        else
        {
            FrameDone();
            return false;
        }
    }

    void FpsSystem::ResetTimer()
    {
        stopwatch.ResetAverage();
        stopwatch.ResetHighest();
    }

    const ExtendedStopwatchAdapter<RealStopwatch>& FpsSystem::GetStopwatch()
    {
        return stopwatch;
    }

    TimeValue FpsSystem::GetFrameStart()
    {
        return start;
    }

    TimeValue FpsSystem::GetFrameEnd()
    {
        return end;
    }

    TimeValue FpsSystem::GetLastFrameTime()
    {
        return stopwatch.QueryElapsed();
    }

    FpsSystem::Fps FpsSystem::GetFps()
    {
        return fps;
    }

    const FpsSystem::Fps FpsSystem::defaultFpsLimit = 60;

    void FpsSystem::InitializeImpl()
    {
        auto initialization = Manager()->FindSystem<InitializationFileSystem>();
        auto fpsLimitEntry = initialization->graphics.CreateEntry<Fps>("FPS Limit", [this]()
        {
            return this->GetFpsLimit();
        });
        fpsLimitEntry->onLoaded.Subscribe([this](Fps value) {
            this->SetFpsLimit(value);
        });

        auto vsyncEntry = initialization->graphics.CreateEntry<bool>("VSync", [this]()
        {
            return this->GetVSync();
        });
        vsyncEntry->onLoaded.Subscribe([this](bool value) {
            this->SetVSync(value);
        });
    }

    bool FpsSystem::IsFrameDone()
    {
        if (currentFpsLimit == 0 || vSync)
            return true;

        return stopwatch.HasReachedGoal();
    }

    void FpsSystem::FrameDone()
    {
        typedef ExtendedStopwatchAdapter<FrameStopwatch> FpsStopwatch;
        // All time values are in nanoseconds, so this needs to initialize from a seconds time value
        static FpsStopwatch fpsTimer(*Manager(), TimeValue(TimeValue::Value(1, 0)));
        static unsigned int count = 0;

        if (fpsTimer.HasReachedGoal())
        {
            fpsTimer.Start();

            fps = count;
            count = 0;
        }

        ++count;
        end = stopwatch.Checkpoint();

        Manager()->FindSystem<TimeSystem>()->OnFrameEnd();
    }
}

namespace Inscription
{
    void Scribe<::Atmos::FpsSystem, BinaryArchive>::Scriven(ObjectT& object, ArchiveT& archive)
    {
        BaseScriven<::Atmos::ObjectSystem>(object, archive);
        archive(object.stopwatch);
    }
}