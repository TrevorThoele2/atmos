
#pragma once

#include "ObjectSystem.h"
#include "DebugStatisticsSystem.h"

#include "InputKey.h"
#include "String.h"

#include <Chroma/Function.h>

#include <AGUI/Root.h>
#include <AGUI/Textbox.h>
#include <AGUI/Label.h>

namespace Atmos
{
    namespace Input { class System; }

    class DebugScreen : public ObjectSystem
    {
    public:
        DebugScreen(ObjectManager& manager);
    private:
        void InitializeImpl() override;
        void WorkImpl() override;
    private:
        Agui::Root* root;
        Agui::Textbox* mainTextbox;
        Agui::TextComponent* label;
        Agui::TextComponent* data;
        Agui::Label* fpsCounter;
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
}

namespace Inscription
{
    template<>
    class Inscripter<::Atmos::DebugScreen> : public InscripterBase<::Atmos::DebugScreen>
    {
    public:
        INSCRIPTION_INSCRIPTER_DECLARE_TABLE;
    };
}