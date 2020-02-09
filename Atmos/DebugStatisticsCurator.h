#pragma once

#include <Arca/Curator.h>

#include "DebugStatistics.h"

namespace Atmos::Debug
{
    class StatisticsCurator final : public Arca::Curator
    {
    public:
        explicit StatisticsCurator(Init init);
    private:
        Arca::GlobalIndex<Statistics> statistics;
    private:
        class PageEntry
        {
        public:
            using RetrievalFunction = std::function<String()>;
            RetrievalFunction retrievalFunction;
        public:
            PageEntry();
            [[nodiscard]] String GetString() const;
        };

        class Page
        {
        public:
            StatisticsCurator& owner;

            Page() = delete;
            explicit Page(StatisticsCurator& owner);

            virtual ~Page() = 0;

            using Entry = PageEntry;
            void AddEntry(const String& label, Entry& add);
        public:
            virtual void Initialize() = 0;
        public:
            [[nodiscard]] String LabelString() const;
            [[nodiscard]] String DataString() const;
        private:
            String label;
            std::vector<Entry*> entries{};
        };

        class ProfilerPage final : public Page
        {
        public:
            struct EntrySet
            {
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
        public:
            using Page::Page;

            void Initialize() override;
        };

        class MemoryPage final : public Page
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
        public:
            using Page::Page;

            void Initialize() override;
        };

        class GamePage final : public Page
        {
        public:
            Entry playerColumn;
            Entry playerRow;
            Entry fieldID;
        public:
            using Page::Page;

            void Initialize() override;
        };

        class WindowPage final : public Page
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
        public:
            using Page::Page;

            void Initialize() override;
        };

        ProfilerPage profilerPage{ *this };
        MemoryPage memoryPage{ *this };
        GamePage gamePage{ *this };
        WindowPage windowPage{ *this };

        using PageList = std::vector<Page*>;
        PageList pageList{};
        void BundlePages();
    private:
        using ProfilerList = std::vector<Statistics::Profiler*>;
        ProfilerList profilerList{};

        void BundleProfilers();
        void ResetProfilerAverage();
        void ResetProfilerHighest();
    };
}

namespace Inscription
{
    template<>
    class Scribe<::Atmos::Debug::StatisticsCurator, BinaryArchive> final :
        public ArcaNullScribe<::Atmos::Debug::StatisticsCurator, BinaryArchive>
    {};
}