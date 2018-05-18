
#include "Fps.h"

#include "FrameTimer.h"
#include "Environment.h"

#include "DayCycle.h"

namespace Atmos
{
    const FpsHandler::FPS FpsHandler::defaultFpsLimit = 60;

    FpsHandler::FpsHandler() : timer(), currentFpsLimit(defaultFpsLimit), fps(0), vSync(false)
    {}

    bool FpsHandler::IsFrameDone()
    {
        if (currentFpsLimit == 0 || vSync)
            return true;

        return timer.HasReachedGoal();
    }

    void FpsHandler::FrameDone()
    {
        typedef ExtendedTimer<FrameTimer> FPSTimer;
        // All time values are in nanoseconds, so this needs to initialize from a seconds time value
        static FPSTimer fpsTimer(TimeValue(TimeValue::ValueT(1, 0)));
        static unsigned int count = 0;

        if (fpsTimer.HasReachedGoal())
        {
            fpsTimer.Start();

            fps = count;
            count = 0;
        }

        ++count;
        end = timer.Checkpoint();

        Environment::GetTime().OnFrameEnd();
        DayCycleHandler::OnFrameEnd();
    }

    FpsHandler& FpsHandler::Instance()
    {
        static FpsHandler instance;
        return instance;
    }

    void FpsHandler::Init()
    {
        SetFpsLimit(*Environment::GetIni().GetEntry<Ini::ID::FPS>());
        SetVSync(*Environment::GetIni().GetEntry<Ini::ID::VSYNC>());
    }

    void FpsHandler::SetFpsLimit(FPS fps)
    {
        auto &currentFpsLimit = Instance().currentFpsLimit;
        currentFpsLimit = fps;
        if (currentFpsLimit == 0)
            Instance().timer.SetGoal(TimeValue::ValueT(0));
        else
            Instance().timer.SetGoal(TimeValue::ValueT(1) / TimeValue::ValueT(currentFpsLimit));
    }

    FpsHandler::FPS FpsHandler::GetFpsLimit()
    {
        return Instance().currentFpsLimit;
    }

    void FpsHandler::SetVSync(bool set)
    {
        Instance().vSync = set;
    }

    bool FpsHandler::GetVSync()
    {
        return Instance().vSync;
    }

    void FpsHandler::StartFrame()
    {
        Instance().start = Instance().timer.Start();
    }

    bool FpsHandler::DoIdle()
    {
        if (!Instance().IsFrameDone())
            return true;
        else
        {
            Instance().FrameDone();
            return false;
        }
    }

    void FpsHandler::ResetTimer()
    {
        auto &timer = Instance().timer;
        timer.ResetAverage();
        timer.ResetHighest();
    }

    const ExtendedTimer<RealTimer>& FpsHandler::GetTimer()
    {
        return Instance().timer;
    }

    TimeValue FpsHandler::GetFrameStart()
    {
        return Instance().start;
    }

    TimeValue FpsHandler::GetFrameEnd()
    {
        return Instance().end;
    }

    TimeValue FpsHandler::GetLastFrameTime()
    {
        return Instance().timer.QueryElapsed();
    }

    FpsHandler::FPS FpsHandler::GetFPS()
    {
        return Instance().fps;
    }
}