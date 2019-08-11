#pragma once

#include "ObjectSystem.h"

#include "ExtendedStopwatchAdapter.h"
#include "RealStopwatch.h"

namespace Atmos::Time
{
    class FpsSystem : public ObjectSystem
    {
    public:
        typedef unsigned int Fps;
    public:
        FpsSystem(ObjectManager& manager);

        void SetFpsLimit(Fps fps);
        Fps GetFpsLimit();
        void SetVSync(bool set);
        bool GetVSync();

        void StartFrame();
        // Returns if idle is still going
        bool DoIdle();

        void ResetTimer();
        const ExtendedStopwatchAdapter<RealStopwatch>& GetStopwatch();

        Value GetFrameStart();
        Value GetFrameEnd();
        Value GetLastFrameTime();

        Fps GetFps();
    protected:
        void InitializeImpl() override;
    private:
        using Stopwatch = ExtendedStopwatchAdapter<RealStopwatch>;
    private:
        bool vSync;
        Fps currentFpsLimit;
        bool idle;
        Fps fps;
        static const Fps defaultFpsLimit;

        Stopwatch stopwatch;
        Value start;
        Value end;
    private:
        bool IsFrameDone();
        void FrameDone();
    private:
        INSCRIPTION_ACCESS;
    };
}

namespace Inscription
{
    template<>
    class Scribe<::Atmos::Time::FpsSystem, BinaryArchive> :
        public ObjectSystemScribe<::Atmos::Time::FpsSystem, BinaryArchive>
    {
    protected:
        void ScrivenImplementation(ObjectT& object, ArchiveT& archive) override;
    };
}
