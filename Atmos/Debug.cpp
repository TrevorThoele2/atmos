
#include <functional>

#include "Debug.h"

#include "Fps.h"
#include "Environment.h"
#include "StateManager.h"
#include "Camera.h"
#include "ImageRegistry.h"
#include "ShaderRegistry.h"
#include "MaterialRegistry.h"
#include "AudioRegistry.h"
#include "ScriptRegistry.h"
#include "ScriptController.h"
#include "DayCycle.h"
#include "StringUtility.h"

#include "Entity.h"
#include "AvatarSystem.h"
#include "EntityPositionSystem.h"
#include "GeneralComponent.h"

#include "FontDefines.h"
#include <AGUI\System.h>
#include <AGUI\SequenceLayout.h>
#include <AGUI\Image.h>

namespace Atmos
{
    DebugScreen::Data::Data(FuncT &&func, bool endingGap) : func(std::move(func)), endingGap(endingGap)
    {}

    String DebugScreen::Data::GetString() const
    {
        String ret(func());
        if (endingGap)
            ret.append("\n");
        return ret;
    }

    void DebugScreen::Page::AddData(const String &label, Data::FuncT &&func, bool endingGap)
    {
        this->label.append(label);
        datums.push_back(Data(std::move(func), endingGap));
    }

    String DebugScreen::Page::GetLabelString() const
    {
        return label;
    }

    String DebugScreen::Page::GetDataString() const
    {
        String ret;
        for (auto &loop : datums)
            ret.append(loop.GetString() + "\n");
        return ret;
    }

    void DebugScreen::Profiler::ResetAverage()
    {
        timer.ResetAverage();
    }

    void DebugScreen::Profiler::ResetHighest()
    {
        timer.ResetHighest();
    }

    TimeValue DebugScreen::Profiler::QueryElapsed() const
    {
        return timer.QueryElapsed();
    }

    TimeValue DebugScreen::Profiler::GetAverage() const
    {
        return timer.GetAverage();
    }

    TimeValue DebugScreen::Profiler::GetHighest() const
    {
        return timer.GetHighest();
    }

    DebugScreen::Profiler::Profiler() : timer()
    {}

    void DebugScreen::Profiler::Start()
    {
        timer.Start();
    }

    void DebugScreen::Profiler::Calc()
    {
        timer.Calculate();
    }

    DebugScreen::DebugScreen() : mainTextbox(nullptr), fpsCounter(nullptr)
    {
        const size_t pageCount = 6;
        for (size_t loop = 0; loop != pageCount; ++loop)
            pages.push_back(Page());

        mousePage = pages.begin() + 0;
        profilerPage = pages.begin() + 1;
        windowPage = pages.begin() + 2;
        memoryPage = pages.begin() + 3;
        playerPage = pages.begin() + 4;
        miscPage = pages.begin() + 5;

        const size_t profilerCount = 7;
        for (size_t loop = 0; loop != profilerCount; ++loop)
            profilers.push_back(Profiler());

        inputProfiler = profilers.begin() + 0;
        logicProfiler = profilers.begin() + 1;
        renderProfiler = profilers.begin() + 2;
        idleProfiler = profilers.begin() + 3;
        misc1Profiler = profilers.begin() + 4;
        misc2Profiler = profilers.begin() + 5;
        misc3Profiler = profilers.begin() + 6;

        mousePage->AddData("Mouse Position X:\n", static_cast<BasicFuncT>([](){ return ToString(Environment::GetInput()->GetMousePosition().x); }), false);
        mousePage->AddData("Mouse Position Y:\n", static_cast<BasicFuncT>([]() { return ToString(Environment::GetInput()->GetMousePosition().y); }), false);
        mousePage->AddData("Mouse Position X in game coordinates:\n", static_cast<BasicFuncT>([]() { return ToString(Environment::GetInput()->GetMousePositionInGameCoords().x); }), false);
        mousePage->AddData("Mouse Position Y in game coordinates:\n", static_cast<BasicFuncT>([]() { return ToString(Environment::GetInput()->GetMousePositionInGameCoords().y); }), false);

        AddProfilerData("Input", *inputProfiler);
        AddProfilerData("Logic", *logicProfiler);
        AddProfilerData("Render", *renderProfiler);
        AddProfilerData("Frame", FpsHandler::GetTimer());
        AddProfilerData("Idle", *idleProfiler);
        AddProfilerData("Misc1", *misc1Profiler);
        AddProfilerData("Misc2", *misc2Profiler);
        AddProfilerData("Misc3", *misc3Profiler);

        profilerPage->AddData("Total elapsed:\n", static_cast<BasicFuncT>([]() { return ToString(Environment::GetTime().GetTotalElapsed()); }), false);

        windowPage->AddData("Window width:\n", static_cast<BasicFuncT>([]() { return ToString(Environment::GetWindowWidth()); }), false);
        windowPage->AddData("Window height:\n", static_cast<BasicFuncT>([]() { return ToString(Environment::GetWindowHeight()); }), false);
        windowPage->AddData("Client width:\n", static_cast<BasicFuncT>([]() { return ToString(Environment::GetClientWidth()); }), false);
        windowPage->AddData("Client height:\n", static_cast<BasicFuncT>([]() { return ToString(Environment::GetClientHeight()); }), false);
        windowPage->AddData("Start X:\n", static_cast<BasicFuncT>([]() { return ToString(Environment::GetStartX()); }), false);
        windowPage->AddData("Start Y:\n", static_cast<BasicFuncT>([]() { return ToString(Environment::GetStartY()); }), false);
        windowPage->AddData("Resolution Width:\n", static_cast<BasicFuncT>([]() { return ToString(agui::System::GetCurrentRes()->GetSize().width); }), false);
        windowPage->AddData("Resolution Height:\n", static_cast<BasicFuncT>([]() { return ToString(agui::System::GetCurrentRes()->GetSize().height); }), false);
        windowPage->AddData("View origin X:\n", static_cast<BasicFuncT>([]() { return ToString(Camera::GetViewOrigin().GetX()); }), false);
        windowPage->AddData("View origin Y:\n", static_cast<BasicFuncT>([]() { return ToString(Camera::GetViewOrigin().GetY()); }), false);

        memoryPage->AddData("Images in memory:\n", static_cast<BasicFuncT>([]() { return ToString(ImageRegistry::Size()); }), false);
        memoryPage->AddData("Shaders in memory:\n", static_cast<BasicFuncT>([]() { return ToString(ShaderRegistry::Size()); }), false);
        memoryPage->AddData("Materials in memory:\n", static_cast<BasicFuncT>([]() { return ToString(MaterialRegistry::Size()); }), false);
        memoryPage->AddData("Audio in memory:\n", static_cast<BasicFuncT>([]() { return ToString(AudioRegistry::Size()); }), false);
        memoryPage->AddData("Scripts in memory:\n\n", static_cast<BasicFuncT>([]() { return ToString(ScriptRegistry::Size()); }), true);

        memoryPage->AddData("Scripts Being Worked:\n", static_cast<BasicFuncT>([]() { return ToString(ScriptController::GetWorkedSize()); }), false);
        memoryPage->AddData("Entities Being Moved:\n", static_cast<BasicFuncT>([]() { return ToString(Ent::PositionSystem::GetWorkedSize()); }), false);
        memoryPage->AddData("Modulators Being Worked:\n", static_cast<BasicFuncT>([]() { return ToString(GameEnvironment::GetModulatorController().GetWorkedSize()); }), false);

        playerPage->AddData("Player col:\n", static_cast<BasicFuncT>([]()
        {
            if (::Atmos::Ent::AvatarSystem::IsValid())
                return ToString(::Atmos::Ent::AvatarSystem::GetPosition().x);
            else
                return ToString(static_cast<GridPosition::ValueT>(0));
        }), false);

        playerPage->AddData("Player row:\n", static_cast<BasicFuncT>([]()
        {
            if (::Atmos::Ent::AvatarSystem::IsValid())
                return ToString(::Atmos::Ent::AvatarSystem::GetPosition().y);
            else
                return ToString(static_cast<GridPosition::ValueT>(0));
        }), false);

        miscPage->AddData("Current in-game time:\n", static_cast<BasicFuncT>([]() { return ToString(DayCycleHandler::GetCurrentTime()); }), false);

        currentPage = pages.begin();
    }

    void DebugScreen::SetupGui()
    {
        root = agui::System::CreateRoot("debug");

        // Debug background
        mainTextbox = agui::Textbox::Factory(root, "debugMainTextbox", agui::RelativePosition(), 0);
        mainTextbox->GetSprite()->ScaleWidthTo(544);
        mainTextbox->GetSprite()->color.Edit(150, 0, 0, 0);
        mainTextbox->CreateLayout<agui::SequenceLayout>(agui::Orientation::HORIZONTAL)->SetMargins(agui::BoundingBox(10, 10, 10, 10));

        auto connection = &mainTextbox->CreateText("label", 1, agui::RelativePosition(agui::Dimension(0, 10), agui::Dimension(0, 10)), agui::Text("", agui::Text::Format(), *agui::fontSlender, agui::Color()));
        connection->GetText().SetAutoCalcTextSize();
        label = &connection->GetText();

        connection = &mainTextbox->CreateText("data", 1, agui::RelativePosition(agui::Dimension(0, 300), agui::Dimension(0, 10)), agui::Text("", agui::Text::Format(), *agui::fontSlender, agui::Color()));
        connection->GetText().SetAutoCalcTextSize();
        data = &connection->GetText();

        // FPS counter
        auto fpsCounterBackground = agui::Image::Factory(root, "fpsCounterImage", agui::RelativePosition(agui::HorizontalAlignment::RIGHT, agui::VerticalAlignment::TOP), 0);
        fpsCounterBackground->GetSprite()->color.Edit(127, 0, 0, 0);
        fpsCounterBackground->ScaleTo(130.0f, 30.0f);

        fpsCounter = agui::Label::Factory(fpsCounterBackground, "fpsCounter", agui::RelativePosition(agui::HorizontalAlignment::MID, agui::VerticalAlignment::MID), 0);
        fpsCounter->ModifyText(agui::Text("", agui::Text::Format({ agui::Text::CENTER_HORIZONTAL, agui::Text::SINGLE_LINE, agui::Text::CENTER_VERTICAL }), *agui::fontSlender, agui::Color()));
        fpsCounter->GetText()->SetAutoCalcTextSize();

        Update();
    }

    void DebugScreen::OnInputDown(const Input::Key &args)
    {
        switch (args.id)
        {
        case Input::KeyID::F1:
            OnF1();
            break;
        case Input::KeyID::F2:
            OnF2();
            break;
        case Input::KeyID::F3:
            OnF3();
            break;
        case Input::KeyID::F4:
            OnF4();
            break;
        }
    }

    void DebugScreen::OnF1()
    {
        ResetProfilerAverage();
        ResetProfilerHighest();

        FpsHandler::ResetTimer();
    }

    void DebugScreen::OnF2()
    {
        ResetProfilerHighest();
    }

    void DebugScreen::OnF3()
    {
        Instance().root->ToggleShown();
    }

    void DebugScreen::OnF4()
    {
        ++currentPage;
        if (currentPage == pages.end())
            currentPage = pages.begin();
    }

    void DebugScreen::AddProfilerData(const String &appendLabel, const Profiler &profiler)
    {
        AddProfilerData(appendLabel, profiler.timer);
    }

    void DebugScreen::AddProfilerData(const String &appendLabel, const Profiler::TimerT &timer)
    {
        typedef typename TimeValue::ValueT ValueT;
        // Add frame time
        profilerPage->AddData(appendLabel + " time:\n",
            Data::FuncT(static_cast<String(*)(const Profiler::TimerT*)>([](const Profiler::TimerT *stopwatch)
            {
                return ToString(stopwatch->QueryElapsed().ConvertValue(TimeValueEpoch::MILLISECONDS));
            }),
                ::function::Bind(&timer)),
            false);
        // Add frame average
        profilerPage->AddData(appendLabel + " time average:\n",
            Data::FuncT(static_cast<String(*)(const Profiler::TimerT*)>([](const Profiler::TimerT *stopwatch)
            {
                return ToString(stopwatch->GetAverage().ConvertValue(TimeValueEpoch::MILLISECONDS));
            }),
                ::function::Bind(&timer)),
            false);
        // Add frame highest
        profilerPage->AddData(appendLabel + " time highest:\n\n",
            Data::FuncT(static_cast<String(*)(const Profiler::TimerT*)>([](const Profiler::TimerT *stopwatch)
            {
                return ToString(stopwatch->GetHighest().ConvertValue(TimeValueEpoch::MILLISECONDS));
            }),
                ::function::Bind(&timer)),
            true);
    }

    void DebugScreen::ResetProfilerAverage()
    {
        for (auto loop = profilers.begin(); loop != profilers.end(); loop++)
            loop->ResetAverage();
    }

    void DebugScreen::ResetProfilerHighest()
    {
        for (auto loop = profilers.begin(); loop != profilers.end(); loop++)
            loop->ResetHighest();
    }

    DebugScreen& DebugScreen::Instance()
    {
        static DebugScreen instance;
        return instance;
    }

    void DebugScreen::Init()
    {
        Instance().SetupGui();
        Environment::GetInput()->eventKeys.pressed.Subscribe(&DebugScreen::OnInputDown, Instance());
    }

    void DebugScreen::Update()
    {
        if (!Instance().root->IsShown())
            return;

        // Page
        String label(Instance().currentPage->GetLabelString());
        String data(Instance().currentPage->GetDataString());
        Instance().label->SetString(label);
        Instance().data->SetString(data);
        // FPS counter
        Instance().fpsCounter->GetText()->SetString("FPS: " + ToString(FpsHandler::GetFPS()));
    }

    DebugScreen::Profiler& DebugScreen::Input()
    {
        return *Instance().inputProfiler;
    }

    DebugScreen::Profiler& DebugScreen::Logic()
    {
        return *Instance().logicProfiler;
    }

    DebugScreen::Profiler& DebugScreen::Render()
    {
        return *Instance().renderProfiler;
    }

    DebugScreen::Profiler& DebugScreen::Idle()
    {
        return *Instance().idleProfiler;
    }

    DebugScreen::Profiler& DebugScreen::Misc1()
    {
        return *Instance().misc1Profiler;
    }

    DebugScreen::Profiler& DebugScreen::Misc2()
    {
        return *Instance().misc2Profiler;
    }

    DebugScreen::Profiler& DebugScreen::Misc3()
    {
        return *Instance().misc3Profiler;
    }
}