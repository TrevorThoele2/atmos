
#pragma once

#include <unordered_map>
#include <string>

#include "ExtendedTimer.h"
#include "RealTimer.h"

namespace Atmos
{
    class FpsHandler
    {
    private:
        typedef ExtendedTimer<RealTimer> Timer;
    public:
        typedef unsigned int FPS;
    private:
        bool vSync;
        FPS currentFpsLimit;
        bool idle;
        FPS fps;
        static const FPS defaultFpsLimit;

        Timer timer;
        TimeValue start;
        TimeValue end;

        FpsHandler();
        bool IsFrameDone();
        void FrameDone();
    public:
        static FpsHandler& Instance();
        static void Init();

        static void SetFpsLimit(FPS fps);
        static FPS GetFpsLimit();
        static void SetVSync(bool set);
        static bool GetVSync();

        static void StartFrame();
        static bool DoIdle(); // Returns if idle is still going

        static void ResetTimer();
        static const ExtendedTimer<RealTimer>& GetTimer();

        static TimeValue GetFrameStart();
        static TimeValue GetFrameEnd();
        static TimeValue GetLastFrameTime();

        static FPS GetFPS();
    };
}