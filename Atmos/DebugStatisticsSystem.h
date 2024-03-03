#pragma once

#include "ObjectSystem.h"

#include "RealStopwatch.h"
#include "ExtendedStopwatchAdapter.h"
#include "FpsSystem.h"

#include "Serialization.h"

namespace Atmos::Time
{
    class TimeSystem;
}

namespace Atmos
{
    class DebugStatisticsSystem : public ObjectSystem
    {
    public:
        DebugStatisticsSystem(ObjectManager& manager);
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
            typedef Time::ExtendedStopwatchAdapter<Time::RealStopwatch> Stopwatch;
            Stopwatch stopwatch;
        private:
            Profiler(ObjectManager& objectManager);

            void ResetAverage();
            void ResetHighest();

            Time::Value QueryElapsed() const;
            Time::Value GetAverage() const;
            Time::Value GetHighest() const;
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
        Time::FpsSystem::Fps Fps();
    private:
        void Setup();
    private:
        PageList pageList;
        void BundlePages();

        typedef std::vector<Profiler*> ProfilerList;
        ProfilerList profilerList;
        void BundleProfilers();
    private:
        Time::FpsSystem* FindFpsSystem();
        Time::TimeSystem* FindTimeSystem();
    };

    template<>
    struct ObjectSystemTraits<DebugStatisticsSystem>
    {
        static const ObjectSystemPriority priority = 0;
    };
}

namespace Inscription
{
    template<>
    class Scribe<::Atmos::DebugStatisticsSystem, BinaryArchive> :
        public ObjectSystemScribe<::Atmos::DebugStatisticsSystem, BinaryArchive>
    {
    protected:
        void ScrivenImplementation(ObjectT& object, ArchiveT& archive) override;
    };
}