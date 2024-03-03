#pragma once

#include "ObjectSystem.h"

#include "RealStopwatch.h"
#include "ExtendedStopwatchAdapter.h"
#include "FpsSystem.h"

#include "Serialization.h"

namespace Atmos
{
    class TimeSystem;

    class DebugStatisticsSystem : public ObjectSystem
    {
    public:
        DebugStatisticsSystem(ObjectManager& manager);
        INSCRIPTION_BINARY_TABLE_CONSTRUCTOR_DEFINE(DebugStatisticsSystem);
    public:
        class PageEntry
        {
        public:
            typedef std::function<String()> RetrievalFunction;
            RetrievalFunction retrievalFunction;
        private:
            PageEntry();
            String GetString() const;
        private:
            friend DebugStatisticsSystem;
        };

        class Page
        {
        public:
            String GetLabelString() const;
            String GetDataString() const;
        protected:
            typedef PageEntry Entry;

            void AddEntry(const String& label, Entry& add);
        private:
            String label;
            std::vector<Entry*> entries;
            Page() = default;
        private:
            friend DebugStatisticsSystem;
        };

        class ProfilerPage : public Page
        {
        public:
            class EntrySet
            {
            public:
                Entry time;
                Entry average;
                Entry highest;
            };
        public:
            EntrySet input;
            EntrySet logic;
            EntrySet render;
            EntrySet frame;
            EntrySet idle;
            EntrySet misc1;
            EntrySet misc2;
            EntrySet misc3;

            Entry totalElapsed;
        private:
            ProfilerPage(DebugStatisticsSystem& owner);
        private:
            friend DebugStatisticsSystem;
        };

        class MemoryPage : public Page
        {
        public:
            Entry audioAssetSize;
            Entry imageAssetSize;
            Entry materialAssetSize;
            Entry scriptAssetSize;
            Entry shaderAssetSize;
            Entry tileSize;
            Entry entitySize;
            Entry workingScriptSize;
            Entry movingEntitySize;
        private:
            MemoryPage(DebugStatisticsSystem& owner);
        private:
            friend DebugStatisticsSystem;
        };

        class GamePage : public Page
        {
        public:
            Entry playerColumn;
            Entry playerRow;
            Entry fieldID;
        private:
            GamePage(DebugStatisticsSystem& owner);
        private:
            friend DebugStatisticsSystem;
        };

        class WindowPage : public Page
        {
        public:
            Entry windowWidth;
            Entry windowHeight;
            Entry clientWidth;
            Entry clientHeight;
            Entry startX;
            Entry startY;
            Entry resolutionWidth;
            Entry resolutionHeight;
            Entry viewOriginX;
            Entry viewOriginY;
        private:
            WindowPage(DebugStatisticsSystem& owner);
        private:
            friend DebugStatisticsSystem;
        };

        class Profiler
        {
        public:
            void Start();
            void Calculate();
        private:
            typedef ExtendedStopwatchAdapter<RealStopwatch> Stopwatch;
            Stopwatch stopwatch;
        private:
            Profiler(ObjectManager& objectManager);

            void ResetAverage();
            void ResetHighest();

            TimeValue QueryElapsed() const;
            TimeValue GetAverage() const;
            TimeValue GetHighest() const;
        private:
            friend DebugStatisticsSystem;
        };
    public:
        typedef std::vector<Page*> PageList;
        typedef std::vector<Profiler*> ProfilerList;
    public:
        ProfilerPage profilerPage;
        MemoryPage memoryPage;
        GamePage gamePage;
        WindowPage windowPage;

        Profiler inputProfiler;
        Profiler logicProfiler;
        Profiler renderProfiler;
        Profiler idleProfiler;
        Profiler misc1Profiler;
        Profiler misc2Profiler;
        Profiler misc3Profiler;

        void ResetProfilerAverage();
        void ResetProfilerHighest();

        const PageList& GetPageList() const;
        FpsSystem::Fps Fps();
    private:
        void Setup();
    private:
        PageList pageList;
        void BundlePages();

        typedef std::vector<Profiler*> ProfilerList;
        ProfilerList profilerList;
        void BundleProfilers();
    private:
        FpsSystem* FindFpsSystem();
        TimeSystem* FindTimeSystem();
    };

    template<>
    struct ObjectSystemTraits<DebugStatisticsSystem>
    {
        static const ObjectSystemPriority priority = 0;
    };
}

namespace Inscription
{
    INSCRIPTION_INSCRIPTER_DECLARE(::Atmos::DebugStatisticsSystem)
    {
    public:
        INSCRIPTION_BINARY_INSCRIPTER_DECLARE_TABLE;
        INSCRIPTION_BINARY_DECLARE_CLASS_NAME_RESOLVER;
    };
}