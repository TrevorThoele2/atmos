
#pragma once

#include "Input.h"
#include "RealTimer.h"
#include "ExtendedTimer.h"
#include "String.h"
#include <Function/Function.h>

#include <AGUI\Root.h>
#include <AGUI\Textbox.h>
#include <AGUI\Label.h>

namespace Atmos
{
    class DebugScreen
    {
    private:
        class Data
        {
        public:
            typedef ::function::Function<String> FuncT;
        private:
            bool endingGap;
            FuncT func;
        public:
            Data(FuncT &&func, bool endingGap);
            String GetString() const;
        };

        class Page
        {
        private:
            String label;
            std::vector<Data> datums;
        public:
            Page() = default;
            void AddData(const String &label, Data::FuncT &&func, bool endingGap);
            String GetLabelString() const;
            String GetDataString() const;
        };
    public:
        class Profiler
        {
        public:
            typedef ExtendedTimer<RealTimer> TimerT;
            friend DebugScreen;
        private:
            TimerT timer;
            void ResetAverage();
            void ResetHighest();
            TimeValue QueryElapsed() const;
            TimeValue GetAverage() const;
            TimeValue GetHighest() const;
        public:
            Profiler();
            void Start();
            void Calc();
            friend DebugScreen;
        };
    private:
        friend Page;
        friend Profiler;
    private:
        typedef String(*BasicFuncT)();

        typedef std::vector<Page> PageVector;
        PageVector pages;
        PageVector::iterator currentPage;
        PageVector::iterator mousePage, profilerPage, windowPage, memoryPage, playerPage, miscPage;

        typedef std::vector<Profiler> ProfilerVector;
        std::vector<Profiler> profilers;
        ProfilerVector::iterator inputProfiler, logicProfiler, renderProfiler, idleProfiler, misc1Profiler, misc2Profiler, misc3Profiler;

        agui::Root *root;
        agui::Textbox *mainTextbox;
        agui::TextComponent *label, *data;
        agui::Label *fpsCounter;

        DebugScreen();
        void SetupGui();

        void OnInputDown(const Input::Key &args);
        void OnF1();
        void OnF2();
        void OnF3();
        void OnF4();

        void AddProfilerData(const String &appendLabel, const Profiler &profiler);
        void AddProfilerData(const String &appendLabel, const Profiler::TimerT &timer);

        void ResetProfilerAverage();
        void ResetProfilerHighest();
    public:
        static DebugScreen& Instance();
        static void Init();
        static void Update();

        static Profiler& Input();
        static Profiler& Logic();
        static Profiler& Render();
        static Profiler& Idle();
        static Profiler& Misc1();
        static Profiler& Misc2();
        static Profiler& Misc3();
    };
}