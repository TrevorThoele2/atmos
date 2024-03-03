#pragma once

#include <Arca/Curator.h>

#include "Stopwatch.h"

#include "String.h"

namespace Atmos::Debug
{
    class StatisticsCurator final : public Arca::Curator
    {
    public:
        explicit StatisticsCurator(Init init);
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
            StatisticsCurator* owner;

            Page() = delete;
            explicit Page(StatisticsCurator& owner);

            virtual ~Page() = 0;

            using Entry = PageEntry;
            void AddEntry(const String& label, Entry& add);
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
            explicit ProfilerPage(StatisticsCurator& owner);
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
            explicit MemoryPage(StatisticsCurator& owner);
        };

        class GamePage final : public Page
        {
        public:
            Entry playerColumn;
            Entry playerRow;
            Entry fieldID;
        public:
            explicit GamePage(StatisticsCurator& owner);
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
            explicit WindowPage(StatisticsCurator& owner);
        };

        ProfilerPage profilerPage{ *this };
        MemoryPage memoryPage{ *this };
        GamePage gamePage{ *this };
        WindowPage windowPage{ *this };

        using PageList = std::vector<Page*>;
        PageList pageList{};
        void BundlePages();
    private:
        using ProfilerList = std::vector<Time::Stopwatch*>;
        ProfilerList profilerList{};

        void BundleProfilers();
        void ResetProfilerAverage();
        void ResetProfilerHighest();
    };
}

namespace Inscription
{
    template<class Archive>
    struct ScribeTraits<Atmos::Debug::StatisticsCurator, Archive> final
    {
        using Category = ArcaNullScribeCategory<Atmos::Debug::StatisticsCurator>;
    };
}