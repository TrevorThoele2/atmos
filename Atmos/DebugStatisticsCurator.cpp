#include "DebugStatisticsCurator.h"

#include <Arca/Reliquary.h>
#include "TimeInformation.h"

#include "StringUtility.h"

namespace Atmos::Debug
{
    StatisticsCurator::PageEntry::PageEntry() : retrievalFunction([]() -> String { return ""; })
    {}

    String StatisticsCurator::PageEntry::GetString() const
    {
        String string(retrievalFunction());
        return string;
    }

    StatisticsCurator::Page::Page(StatisticsCurator& owner) : owner(owner)
    {}

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

    void StatisticsCurator::ProfilerPage::Initialize()
    {
        const auto entryAdder = [this](const String& name, const Statistics::Profiler& profiler, EntrySet& entrySet)
        {
            AddEntry(name + " time", entrySet.time);
            entrySet.time.retrievalFunction = [&profiler]() -> String
            {
                return ToString(profiler.QueryElapsed().GetAs(Time::Epoch::Milliseconds));
            };

            AddEntry(name + " average", entrySet.average);
            entrySet.average.retrievalFunction = [&profiler]() -> String
            {
                return ToString(profiler.GetAverage().GetAs(Time::Epoch::Milliseconds));
            };

            AddEntry(name + " highest", entrySet.highest);
            entrySet.highest.retrievalFunction = [&profiler]() -> String
            {
                return ToString(profiler.GetHighest().GetAs(Time::Epoch::Milliseconds));
            };
        };

        const auto statistics = owner.statistics;

        entryAdder("Input", statistics->profilers.input, input);
        entryAdder("Logic", statistics->profilers.logic, logic);
        entryAdder("Render", statistics->profilers.render, render);
        entryAdder("Frame", statistics->profilers.frameTime, frame);
        entryAdder("Idle", statistics->profilers.idle, idle);
        entryAdder("Misc1", statistics->profilers.misc1, misc1);
        entryAdder("Misc2", statistics->profilers.misc2, misc2);
        entryAdder("Misc3", statistics->profilers.misc3, misc3);

        AddEntry("Total Elapsed", totalElapsed);
        auto timeInformation = Arca::GlobalPtr<Time::Information>(owner.Owner());
        totalElapsed.retrievalFunction = [&timeInformation]() -> String
        {
            return ToString(timeInformation->totalElapsed);
        };
    }

    void StatisticsCurator::MemoryPage::Initialize()
    {
        AddEntry("Audio Count", audioAssetSize);
        AddEntry("Image Count", imageAssetSize);
        AddEntry("Material Count", imageAssetSize);
        AddEntry("Script Count", imageAssetSize);
        AddEntry("Shader Count", imageAssetSize);
        AddEntry("Tile Count", tileSize);
        AddEntry("Entity Count", entitySize);
        AddEntry("Working Script Count", workingScriptSize);
        AddEntry("Moving Entity Count", movingEntitySize);
    }

    void StatisticsCurator::GamePage::Initialize()
    {
        AddEntry("Player Column", playerColumn);
        AddEntry("Player Row", playerColumn);
        AddEntry("Field ID", playerColumn);
    }

    void StatisticsCurator::WindowPage::Initialize()
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

    void StatisticsCurator::InitializeImplementation()
    {
        statistics = Arca::GlobalPtr<Statistics>(Owner());

        BundlePages();
        BundleProfilers();

        for (auto& loop : pageList)
            loop->Initialize();
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
        profilerList.push_back(&statistics->profilers.input);
        profilerList.push_back(&statistics->profilers.logic);
        profilerList.push_back(&statistics->profilers.render);
        profilerList.push_back(&statistics->profilers.frameTime);
        profilerList.push_back(&statistics->profilers.idle);
        profilerList.push_back(&statistics->profilers.misc1);
        profilerList.push_back(&statistics->profilers.misc2);
        profilerList.push_back(&statistics->profilers.misc3);
    }

    void StatisticsCurator::ResetProfilerAverage()
    {
        for (auto& loop : profilerList)
            loop->ResetAverage();
    }

    void StatisticsCurator::ResetProfilerHighest()
    {
        for (auto& loop : profilerList)
            loop->ResetHighest();
    }
}