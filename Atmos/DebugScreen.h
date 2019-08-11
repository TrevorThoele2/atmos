
#pragma once

#include "ObjectSystem.h"
#include "DebugStatisticsSystem.h"

#include "InputKey.h"
#include "String.h"

#include <Chroma/Function.h>

namespace Atmos
{
    class DebugScreen : public ObjectSystem
    {
    public:
        DebugScreen(ObjectManager& manager);
    };

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
        
        typedef DebugStatisticsSystem StatisticsSystem;
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
        FpsSystem* FindFpsSystem();
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
    template<>
    class Scribe<::Atmos::DebugScreen, BinaryArchive> :
        public ObjectSystemScribe<::Atmos::DebugScreen, BinaryArchive>
    {
    protected:
        void ScrivenImplementation(ObjectT& object, ArchiveT& archive) override;
    };
}