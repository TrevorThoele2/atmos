#pragma once

#include <Arca/Curator.h>

#include "String.h"

#include <Chroma/Function.h>

namespace Atmos
{
    class DebugScreen : public Arca::Curator
    {};

    /*
    namespace Input { class System; }

    class DebugScreen : public ObjectSystem
    {
    public:
        DebugScreen(ObjectManager& manager);
    private:
        void InitializeImpl() override;
        void WorkImpl() override;
    private:
        int currentPageIndex;
        
        typedef StatisticsSystem StatisticsSystem;
        typedef StatisticsSystem::Page Page;
        Page* currentPage;

        void IncrementCurrentPage();
    private:
        void SetupGui();
    private:
        void ResetProfilerAverage();
        void ResetProfilerHighest();
    private:
        void OnInputDown(const Input::Key& args);
        void OnF1();
        void OnF2();
        void OnF3();
        void OnF4();
    private:
        StatisticsSystem* FindStatisticsSystem();
        TimeSystem* FindFpsSystem();
        Input::System* FindInputSystem();
    };

    template<>
    struct ObjectSystemTraits<DebugScreen>
    {
        static const ObjectSystemPriority priority = 0;
    };
    */
}

namespace Inscription
{
    template<class Archive>
    struct ScribeTraits<Atmos::DebugScreen, Archive> final
    {
        using Category = ArcaNullScribeCategory<Atmos::DebugScreen>;
    };
}