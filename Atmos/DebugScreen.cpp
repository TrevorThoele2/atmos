
#include <functional>

#include "DebugScreen.h"

#include "ObjectManager.h"
#include "InputSystem.h"

#include "StringUtility.h"

#include "FontDefines.h"

namespace Atmos
{
    DebugScreen::DebugScreen(ObjectManager& manager) : ObjectSystem(manager)
    {}

    /*
    template<class T>
    T* RetrieveFromLocalObjectManager()
    {
        auto objectManager = GetLocalObjectManager();
        if (!objectManager)
            return nullptr;

        return objectManager->FindSystem<T>();
    }

    template<class T>
    T* RetrieveFromGlobalObjectManager()
    {
        auto objectManager = GetGlobalObjectManager();
        return objectManager->FindSystem<T>();
    }

    template<class SystemT>
    String ObjectSystemInformation(
        ObjectManager& objectManager,
        std::function<String(const SystemT&)>&& whenSystemExists,
        const String& whenSystemNotExists)
    {
        auto system = objectManager.FindSystem<SystemT>();
        if (system == nullptr)
            return whenSystemNotExists;

        return whenSystemExists(*system);
    }

    DebugScreen::DebugScreen(ObjectManager& manager) :
        ObjectSystem(manager), mainTextbox(nullptr), fpsCounter(nullptr), currentPage(0)
    {}

    void DebugScreen::InitializeImpl()
    {
        SetupGui();
        FindInputSystem()->Get()->eventKeys.pressed.Subscribe(&DebugScreen::OnInputDown, *this);
    }

    void DebugScreen::WorkImpl()
    {
        if (!root->IsShown())
            return;

        String label(currentPage->GetLabelString());
        String data(currentPage->GetDataString());
        this->label->SetString(label);
        this->data->SetString(data);

        auto statistics = FindStatisticsSystem();
        fpsCounter->GetText()->SetString("FPS: " + ToString(statistics->Fps()));
    }

    void DebugScreen::IncrementCurrentPage()
    {
        auto list = FindStatisticsSystem()->GetPageList();
        
        ++currentPageIndex;
        if (currentPageIndex > static_cast<int>(list.size() - 1))
            currentPageIndex = 0;

        currentPage = list[currentPageIndex];
    }

    void DebugScreen::SetupGui()
    {
        root = Agui::System::CreateRoot("debug");

        // Debug background
        mainTextbox = Agui::Textbox::Factory(root, "debugMainTextbox", Agui::RelativePosition(), 0);
        mainTextbox->GetSprite()->ScaleWidthTo(544);
        mainTextbox->GetSprite()->color.Edit(150, 0, 0, 0);
        mainTextbox->CreateLayout<Agui::SequenceLayout>(Agui::Orientation::HORIZONTAL)->SetMargins(Agui::BoundingBox(10, 10, 10, 10));

        auto connection = &mainTextbox->CreateText(
            "label",
            1,
            Agui::RelativePosition(Agui::Dimension(0, 10), Agui::Dimension(0, 10)),
            Agui::Text("", Agui::Text::Format(), *Agui::fontSlender, Agui::Color()));
        connection->GetText().SetAutoCalcTextSize();
        label = &connection->GetText();

        connection = &mainTextbox->CreateText(
            "data",
            1,
            Agui::RelativePosition(Agui::Dimension(0, 300), Agui::Dimension(0, 10)),
            Agui::Text("", Agui::Text::Format(), *Agui::fontSlender, Agui::Color()));
        connection->GetText().SetAutoCalcTextSize();
        data = &connection->GetText();

        // FPS counter
        auto fpsCounterBackground = Agui::Image::Factory(
            root,
            "fpsCounterImage",
            Agui::RelativePosition(Agui::HorizontalAlignment::RIGHT,Agui::VerticalAlignment::TOP),
            0);
        fpsCounterBackground->GetSprite()->color.Edit(127, 0, 0, 0);
        fpsCounterBackground->ScaleTo(130.0f, 30.0f);

        fpsCounter = Agui::Label::Factory(
            fpsCounterBackground,
            "fpsCounter",
            Agui::RelativePosition(Agui::HorizontalAlignment::MID, Agui::VerticalAlignment::MID),
            0);
        fpsCounter->ModifyText(Agui::Text(
            "",
            Agui::Text::Format{ Agui::Text::CENTER_HORIZONTAL, Agui::Text::SINGLE_LINE, Agui::Text::CENTER_VERTICAL },
            *Agui::fontSlender,
            Agui::Color()));
        fpsCounter->GetText()->SetAutoCalcTextSize();
    }

    void DebugScreen::ResetProfilerAverage()
    {
        FindStatisticsSystem()->ResetProfilerAverage();
    }

    void DebugScreen::ResetProfilerHighest()
    {
        FindStatisticsSystem()->ResetProfilerHighest();
    }

    void DebugScreen::OnInputDown(const Input::Key& args)
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
        FindFpsSystem()->ResetTimer();
    }

    void DebugScreen::OnF2()
    {
        ResetProfilerHighest();
    }

    void DebugScreen::OnF3()
    {
        root->ToggleShown();
    }

    void DebugScreen::OnF4()
    {
        IncrementCurrentPage();
    }

    DebugScreen::StatisticsSystem* DebugScreen::FindStatisticsSystem()
    {
        return Manager()->FindSystem<StatisticsSystem>();
    }

    FpsSystem* DebugScreen::FindFpsSystem()
    {
        return Manager()->FindSystem<FpsSystem>();
    }

    Input::System* DebugScreen::FindInputSystem()
    {
        return Manager()->FindSystem<Input::System>();
    }
    */
}

namespace Inscription
{
    void Scribe<::Atmos::DebugScreen, BinaryArchive>::Scriven(ObjectT& object, ArchiveT& archive)
    {
        BaseScriven<::Atmos::ObjectSystem>(object, archive);
    }
}