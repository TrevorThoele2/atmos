
#include "DebugStatisticsSystem.h"

#include "ObjectManager.h"
#include "TimeSystem.h"

#include "StringUtility.h"

namespace Atmos
{
    DebugStatisticsSystem::DebugStatisticsSystem(ObjectManager& manager) :
        ObjectSystem(manager), profilerPage(*this), memoryPage(*this), gamePage(*this), windowPage(*this),
        inputProfiler(manager), logicProfiler(manager), renderProfiler(manager), idleProfiler(manager),
        misc1Profiler(manager), misc2Profiler(manager), misc3Profiler(manager)
    {
        Setup();
    }

    INSCRIPTION_BINARY_TABLE_CONSTRUCTOR_DEFINE(DebugStatisticsSystem) :
        INSCRIPTION_TABLE_GET_BASE(ObjectSystem), profilerPage(*this), memoryPage(*this), gamePage(*this), windowPage(*this),
        inputProfiler(*Manager()), logicProfiler(*Manager()), renderProfiler(*Manager()), idleProfiler(*Manager()),
        misc1Profiler(*Manager()), misc2Profiler(*Manager()), misc3Profiler(*Manager())
    {
        Setup();
    }

    DebugStatisticsSystem::PageEntry::PageEntry() : retrievalFunction([]() -> String { return ""; })
    {}

    String DebugStatisticsSystem::PageEntry::GetString() const
    {
        String ret(retrievalFunction());
        return ret;
    }

    String DebugStatisticsSystem::Page::GetLabelString() const
    {
        return label;
    }

    String DebugStatisticsSystem::Page::GetDataString() const
    {
        String ret;
        for (auto& loop : entries)
            ret.append(loop->GetString() + "\n");
        return ret;
    }

    void DebugStatisticsSystem::Page::AddEntry(const String& label, Entry& add)
    {
        this->label.append(label);
        entries.push_back(&add);
    }

    DebugStatisticsSystem::ProfilerPage::ProfilerPage(DebugStatisticsSystem& owner)
    {
        auto entryAdder = [this](const String& name, const Profiler::Stopwatch& stopwatch, EntrySet& entrySet)
        {
            AddEntry(name + " time", entrySet.time);
            entrySet.time.retrievalFunction = [&stopwatch]() -> String
            {
                return ToString(stopwatch.QueryElapsed().ConvertValue(TimeValueEpoch::MILLISECONDS));
            };

            AddEntry(name + " average", entrySet.average);
            entrySet.average.retrievalFunction = [&stopwatch]() -> String
            {
                return ToString(stopwatch.GetAverage().ConvertValue(TimeValueEpoch::MILLISECONDS));
            };

            AddEntry(name + " highest", entrySet.highest);
            entrySet.highest.retrievalFunction = [&stopwatch]() -> String
            {
                return ToString(stopwatch.GetHighest().ConvertValue(TimeValueEpoch::MILLISECONDS));
            };
        };

        auto fpsSystem = owner.FindFpsSystem();

        entryAdder("Input", owner.inputProfiler.stopwatch, input);
        entryAdder("Logic", owner.logicProfiler.stopwatch, logic);
        entryAdder("Render", owner.renderProfiler.stopwatch, render);
        entryAdder("Frame", fpsSystem->GetStopwatch(), frame);
        entryAdder("Idle", owner.idleProfiler.stopwatch, idle);
        entryAdder("Misc1", owner.misc1Profiler.stopwatch, misc1);
        entryAdder("Misc2", owner.misc2Profiler.stopwatch, misc2);
        entryAdder("Misc3", owner.misc3Profiler.stopwatch, misc3);

        auto timeSystem = owner.FindTimeSystem();

        AddEntry("Total Elapsed", totalElapsed);
        totalElapsed.retrievalFunction = [timeSystem]() -> String
        {
            return ToString(timeSystem->GetTotalElapsed());
        };
    }

    DebugStatisticsSystem::MemoryPage::MemoryPage(DebugStatisticsSystem& owner)
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

    DebugStatisticsSystem::GamePage::GamePage(DebugStatisticsSystem& owner)
    {
        AddEntry("Player Column", playerColumn);
        AddEntry("Player Row", playerColumn);
        AddEntry("Field ID", playerColumn);
    }

    DebugStatisticsSystem::WindowPage::WindowPage(DebugStatisticsSystem& owner)
    {
        AddEntry("Window Width", windowWidth);
        AddEntry("Window Height", windowHeight);
        AddEntry("Client Width", clientWidth);
        AddEntry("Client Height", clientHeight);
        AddEntry("Start X", startX);
        AddEntry("Start Y", startY);
        AddEntry("Resolution Width", resolutionWidth);
        AddEntry("Resolution Height", resolutionHeight);
        AddEntry("View Origin X", viewOriginX);
        AddEntry("View Origin Y", viewOriginY);
    }

    void DebugStatisticsSystem::Profiler::Start()
    {
        stopwatch.Start();
    }

    void DebugStatisticsSystem::Profiler::Calculate()
    {
        stopwatch.Calculate();
    }

    DebugStatisticsSystem::Profiler::Profiler(ObjectManager& objectManager) :
        stopwatch(objectManager)
    {}

    void DebugStatisticsSystem::Profiler::ResetAverage()
    {
        stopwatch.ResetAverage();
    }

    void DebugStatisticsSystem::Profiler::ResetHighest()
    {
        stopwatch.ResetHighest();
    }

    TimeValue DebugStatisticsSystem::Profiler::QueryElapsed() const
    {
        return stopwatch.QueryElapsed();
    }

    TimeValue DebugStatisticsSystem::Profiler::GetAverage() const
    {
        return stopwatch.GetAverage();
    }

    TimeValue DebugStatisticsSystem::Profiler::GetHighest() const
    {
        return stopwatch.GetHighest();
    }

    void DebugStatisticsSystem::ResetProfilerAverage()
    {
        for (auto& loop : profilerList)
            loop->ResetAverage();
    }

    void DebugStatisticsSystem::ResetProfilerHighest()
    {
        for (auto& loop : profilerList)
            loop->ResetHighest();
    }

    const DebugStatisticsSystem::PageList& DebugStatisticsSystem::GetPageList() const
    {
        return pageList;
    }

    FpsSystem::Fps DebugStatisticsSystem::Fps()
    {
        return Manager()->FindSystem<FpsSystem>()->GetFps();
    }

    void DebugStatisticsSystem::Setup()
    {
        BundlePages();
        BundleProfilers();
    }

    void DebugStatisticsSystem::BundlePages()
    {
        pageList.push_back(&profilerPage);
        pageList.push_back(&gamePage);
        pageList.push_back(&memoryPage);
        pageList.push_back(&windowPage);
    }

    void DebugStatisticsSystem::BundleProfilers()
    {
        profilerList.push_back(&inputProfiler);
        profilerList.push_back(&logicProfiler);
        profilerList.push_back(&renderProfiler);
        profilerList.push_back(&idleProfiler);
        profilerList.push_back(&misc1Profiler);
        profilerList.push_back(&misc2Profiler);
        profilerList.push_back(&misc3Profiler);
    }

    FpsSystem* DebugStatisticsSystem::FindFpsSystem()
    {
        return Manager()->FindSystem<FpsSystem>();
    }

    TimeSystem* DebugStatisticsSystem::FindTimeSystem()
    {
        return Manager()->FindSystem<TimeSystem>();
    }
}

namespace Inscription
{
    INSCRIPTION_BINARY_INSCRIPTER_DEFINE_TABLE(::Atmos::DebugStatisticsSystem)
    {
        INSCRIPTION_BINARY_INSCRIPTER_CREATE_TABLE;

        INSCRIPTION_TABLE_ADD_BASE(::Atmos::ObjectSystem);

        INSCRIPTION_INSCRIPTER_RETURN_TABLE;
    }

    INSCRIPTION_BINARY_DEFINE_SIMPLE_CLASS_NAME_RESOLVER(::Atmos::DebugStatisticsSystem, "DebugStatisticsSystem");
}