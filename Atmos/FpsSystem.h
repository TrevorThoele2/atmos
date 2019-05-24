#pragma once

#include "ObjectSystem.h"

#include "ExtendedStopwatchAdapter.h"
#include "RealStopwatch.h"

namespace Atmos
{
    class FpsSystem : public ObjectSystem
    {
    public:
        typedef unsigned int Fps;
    public:
        FpsSystem(ObjectManager& manager);
        INSCRIPTION_BINARY_TABLE_CONSTRUCTOR_DECLARE(FpsSystem);

        void SetFpsLimit(Fps fps);
        Fps GetFpsLimit();
        void SetVSync(bool set);
        bool GetVSync();

        void StartFrame();
        // Returns if idle is still going
        bool DoIdle();

        void ResetTimer();
        const ExtendedStopwatchAdapter<RealStopwatch>& GetStopwatch();

        TimeValue GetFrameStart();
        TimeValue GetFrameEnd();
        TimeValue GetLastFrameTime();

        Fps GetFps();
    private:
        typedef ExtendedStopwatchAdapter<RealStopwatch> Stopwatch;
    private:
        bool vSync;
        Fps currentFpsLimit;
        bool idle;
        Fps fps;
        static const Fps defaultFpsLimit;

        Stopwatch stopwatch;
        TimeValue start;
        TimeValue end;
    private:
        void InitializeImpl() override;
    private:
        bool IsFrameDone();
        void FrameDone();
    private:
        INSCRIPTION_ACCESS;
    };
}

namespace Inscription
{
    INSCRIPTION_INSCRIPTER_DECLARE(::Atmos::FpsSystem)
    {
    public:
        INSCRIPTION_BINARY_INSCRIPTER_DECLARE_TABLE;
    };
}
