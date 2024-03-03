
#include "ControlsState.h"

#include "StateSystem.h"

#include "InputSignal.h"
#include "InputAction.h"
#include "InputSystem.h"

#include "ObjectManager.h"
#include "InitializationFileSystem.h"

#include "StringUtility.h"

#include "FontDefines.h"
#include <AGUI/Image.h>
#include <AGUI/Label.h>

namespace Atmos
{
    ControlsStateGui::ControlsStateGui(ObjectManager& objectManager) :
        StateGui(objectManager, "controlsScreen"), pressedButton(nullptr)
    {}

    ControlsStateGui::Button::Button(Agui::PushButton& widget, Input::Action& action) : widget(widget), action(action), requested(nullptr)
    {}

    void ControlsStateGui::Button::Request(const Input::SignalBase& request)
    {
        widget.GetText()->SetString(request.displayName);
        requested = &request;
    }

    void ControlsStateGui::Button::Reset()
    {
        widget.GetText()->SetString(action.MappedSignal()->displayName);
        requested = nullptr;
    }

    void ControlsStateGui::Button::LockIn()
    {
        if (requested)
            action.MapToSignal(requested);

        Reset();
    }

    void ControlsStateGui::DoInitialize()
    {
        auto inputManager = FindInputSystem()->Get();
        inputManager->eventKeys.pressed.Subscribe(&ControlsStateGui::OnInputPressed, *this);

        auto root = Root();

        // Background
        auto background = Agui::Image::Factory(
            root,
            "background",
            Agui::RelativePosition(
                Agui::HorizontalAlignment::MID,
                Agui::VerticalAlignment::MID),
            0);
        background->GetSprite()->ScaleTo(1024, 768);
        background->GetSprite()->color.Edit(255, 100, 100, 255);
        Agui::CreateRootResFitter(*background);

        buttons.reserve(inputManager->GetActions().size());
        // Left most column
        for (auto& loop : inputManager->GetActions())
            AddButton(*loop);

        // Accept button
        auto button = Agui::PushButton::Factory(
            root,
            "accept",
            Agui::RelativePosition(
                Agui::Dimension(0, 60),
                Agui::Dimension(0, -48),
                Agui::HorizontalAlignment::LEFT,
                Agui::VerticalAlignment::BOT),
            1);
        button->GetText()->SetString("ACCEPT");
        button->GetText()->color = Agui::colorBlack;
        button->eventClicked.Subscribe(std::bind(&ControlsStateGui::OnAcceptButtonClicked, this));

        // Cancel button
        button = Agui::PushButton::Factory(
            root,
            "cancel",
            Agui::RelativePosition(
                Agui::Dimension(0, -60),
                Agui::Dimension(0, -48),
                Agui::HorizontalAlignment::RIGHT,
                Agui::VerticalAlignment::BOT),
            1);
        button->GetText()->SetString("CANCEL");
        button->GetText()->color = Agui::colorBlack;
        button->eventClicked.Subscribe(std::bind(&ControlsStateGui::OnCancelButtonClicked, this));
    }

    void ControlsStateGui::SetButtonPressed(Button* setTo)
    {
        pressedButton = setTo;
    }

    void ControlsStateGui::OnInputPressed(const Input::Key& key)
    {
        if (pressedButton && !FindInputSystem()->Get()->IsKeyUsedForAction(key) && key.CanUseForAction())
        {
            pressedButton->Request(key);
            pressedButton = nullptr;
        }
    }

    void ControlsStateGui::OnAcceptButtonClicked()
    {
        for (auto& loop : buttons)
            loop.LockIn();

        FindInitializationFileSystem()->Save();
        FindStateSystem()->Pop();
    }

    void ControlsStateGui::OnCancelButtonClicked()
    {
        for (auto& loop : buttons)
            loop.Reset();

        FindStateSystem()->Pop();
    }

    void ControlsStateGui::AddButton(Input::Action& action)
    {
        static const float startX = 30;
        static const float startY = 30;
        static const float incrementY = 60;
        static const float thresholdY = 510;

        static const float labelDifferenceY = -20;

        auto totalX = startX;
        auto totalY = startY;
        Agui::HorizontalAlignment horizAlign(Agui::HorizontalAlignment::LEFT);
        unsigned short count = 1;

        auto root = Root();

        // Create push button and new ControlsButton
        auto pushButton = Agui::PushButton::Factory(
            root,
            "button" + ToString(count),
            Agui::RelativePosition(
                Agui::Dimension(0, totalX),
                Agui::Dimension(0, totalY),
                horizAlign,
                Agui::VerticalAlignment::TOP),
            1);
        buttons.push_back(Button(*pushButton, action));

        // Setup push button
        pushButton->ModifySprite(Agui::Sprite("buttonSmall.png", 1, Agui::Color()));
        pushButton->GetText()->SetString(action.MappedSignal()->displayName);
        pushButton->GetText()->color = Agui::colorBlack;
        pushButton->eventClicked.Subscribe(std::bind(&ControlsStateGui::SetButtonPressed, this, &buttons.back()));

        // Label
        auto label = Agui::Label::Factory(pushButton, "label", Agui::RelativePosition(Agui::Dimension(), Agui::Dimension(0, labelDifferenceY)), 0);
        label->ModifyText(Agui::Text(action.displayName + ":", Agui::Text::WORD_BREAK, *Agui::fontSlender, Agui::colorBlack));
        label->GetText()->SetAutoCalcTextSize();

        // Handle x and y positions
        totalY += incrementY;
        if (totalY > thresholdY)
        {
            totalY = startY;

            if (horizAlign == Agui::HorizontalAlignment::LEFT)
            {
                horizAlign = Agui::HorizontalAlignment::MID;
                totalX = 0;
            }
            else if (horizAlign == Agui::HorizontalAlignment::MID)
            {
                horizAlign = Agui::HorizontalAlignment::RIGHT;
                totalX = startX;
            }
        }

        ++count;
    }

    Input::System* ControlsStateGui::FindInputSystem()
    {
        return objectManager->FindSystem<Input::System>();
    }

    StateSystem* ControlsStateGui::FindStateSystem()
    {
        return objectManager->FindSystem<StateSystem>();
    }

    InitializationFileSystem* ControlsStateGui::FindInitializationFileSystem()
    {
        return objectManager->FindSystem<InitializationFileSystem>();
    }

    const ObjectTypeName ObjectTraits<ControlsState>::typeName = "ControlsState";
}

namespace Inscription
{
    OBJECT_INSCRIPTER_DEFINE_MEMBERS(::Atmos::ControlsState)
    {

    }
}