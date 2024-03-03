#include "DebugStatisticsCurator.h"

#include "DebugStatistics.h"

#include <Arca/Reliquary.h>
#include "FrameInformation.h"

#include "StringUtility.h"

namespace Atmos::Debug
{
    StatisticsCurator::StatisticsCurator(Init init) : Curator(init)
    {
        BundlePages();
        BundleProfilers();
    }

    StatisticsCurator::PageEntry::PageEntry() : retrievalFunction([]() -> String { return ""; })
    {}

    String StatisticsCurator::PageEntry::GetString() const
    {
        String string(retrievalFunction());
        return string;
    }
    
    StatisticsCurator::Page::~Page() = default;

    void StatisticsCurator::Page::AddEntry(const String& label, Entry& add)
    {
        this->label.append(label);
        entries.push_back(&add);
    }

    String StatisticsCurator::Page::LabelString() const
    {
        return label;
    }

    String StatisticsCurator::Page::DataString() const
    {
        String ret;
        for (auto& loop : entries)
            ret.append(loop->GetString() + "\n");
        return ret;
    }

    StatisticsCurator::ProfilerPage::ProfilerPage(StatisticsCurator& owner)
    {
        const auto entryAdder = [this](const String& name, const Time::Stopwatch& stopwatch, EntrySet& entrySet)
        {
            AddEntry(name + " time", entrySet.time);
            entrySet.time.retrievalFunction = [&stopwatch]() -> String
            {
                return ToString(std::chrono::duration_cast<Time::Milliseconds>(stopwatch.Elapsed()));
            };

            AddEntry(name + " average", entrySet.average);
            entrySet.average.retrievalFunction = [&stopwatch]() -> String
            {
                return ToString(std::chrono::duration_cast<Time::Milliseconds>(stopwatch.Average()));
            };

            AddEntry(name + " highest", entrySet.highest);
            entrySet.highest.retrievalFunction = [&stopwatch]() -> String
            {
                return ToString(std::chrono::duration_cast<Time::Milliseconds>(stopwatch.Highest()));
            };
        };

        const auto statistics = Arca::Index<Statistics>(owner.Owner());

        entryAdder("Input", statistics->profilers.input, input);
        entryAdder("Logic", statistics->profilers.logic, logic);
        entryAdder("Render", statistics->profilers.render, render);
        entryAdder("Frame", statistics->profilers.frameTime, frame);
        entryAdder("Idle", statistics->profilers.idle, idle);
        entryAdder("Misc1", statistics->profilers.misc1, misc1);
        entryAdder("Misc2", statistics->profilers.misc2, misc2);
        entryAdder("Misc3", statistics->profilers.misc3, misc3);

        AddEntry("Total Elapsed", totalElapsed);
        auto frameInformation = Arca::Index<Frame::Information>(owner.Owner());
        totalElapsed.retrievalFunction = [&frameInformation]() -> String
        {
            return ToString(frameInformation->totalElapsed);
        };
    }

    StatisticsCurator::MemoryPage::MemoryPage(StatisticsCurator& owner)
    {
        AddEntry("Audio Count", audioAssetSize);
        AddEntry("Image Count", imageAssetSize);
        AddEntry("Material Count", materialAssetSize);
        AddEntry("Script Count", scriptAssetSize);
        AddEntry("Shader Count", shaderAssetSize);
        AddEntry("Tile Count", tileSize);
        AddEntry("Entity Count", entitySize);
        AddEntry("Working Script Count", workingScriptSize);
        AddEntry("Moving Entity Count", movingEntitySize);
    }

    StatisticsCurator::GamePage::GamePage(StatisticsCurator& owner)
    {
        AddEntry("Player Column", playerColumn);
        AddEntry("Player Row", playerColumn);
        AddEntry("Field ID", playerColumn);
    }

    StatisticsCurator::WindowPage::WindowPage(StatisticsCurator& owner)
    {
        AddEntry("Window Width", windowWidth);
        AddEntry("Window Height", windowHeight);
        AddEntry("Client Width", clientWidth);
        AddEntry("Client Height", clientHeight);
        AddEntry("WorldStart X", startX);
        AddEntry("WorldStart Y", startY);
        AddEntry("Resolution Width", resolutionWidth);
        AddEntry("Resolution Height", resolutionHeight);
        AddEntry("View Origin X", viewOriginX);
        AddEntry("View Origin Y", viewOriginY);
    }

    void StatisticsCurator::BundlePages()
    {
        pageList.push_back(&profilerPage);
        pageList.push_back(&gamePage);
        pageList.push_back(&memoryPage);
        pageList.push_back(&windowPage);
    }

    void StatisticsCurator::BundleProfilers()
    {
        auto mutableStatistics = MutablePointer().Of<Statistics>();
        profilerList.push_back(&mutableStatistics->profilers.input);
        profilerList.push_back(&mutableStatistics->profilers.logic);
        profilerList.push_back(&mutableStatistics->profilers.render);
        profilerList.push_back(&mutableStatistics->profilers.frameTime);
        profilerList.push_back(&mutableStatistics->profilers.idle);
        profilerList.push_back(&mutableStatistics->profilers.misc1);
        profilerList.push_back(&mutableStatistics->profilers.misc2);
        profilerList.push_back(&mutableStatistics->profilers.misc3);
    }

    void StatisticsCurator::ResetProfilerAverage()
    {
        for (auto& profiler : profilerList)
            profiler->ResetAverage();
    }

    void StatisticsCurator::ResetProfilerHighest()
    {
        for (auto& profiler : profilerList)
            profiler->ResetHighest();
    }
}
