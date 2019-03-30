
#include "Controls.h"

#include "Environment.h"
#include "InputAction.h"

#include "StateManager.h"

#include "StringUtility.h"

#include "FontDefines.h"
#include <AGUI\Image.h>
#include <AGUI\Label.h>

namespace Atmos
{
    ControlsScreenGui::Button::Button(Agui::PushButton &widget, Input::Action &action) : widget(widget), action(action), requested(nullptr)
    {}

    void ControlsScreenGui::Button::Request(const Input::SignalBase &request)
    {
        widget.GetText()->SetString(request.displayName);
        requested = &request;
    }

    void ControlsScreenGui::Button::Reset()
    {
        widget.GetText()->SetString(action.GetMappedKey()->displayName);
        requested = nullptr;
    }

    void ControlsScreenGui::Button::LockIn()
    {
        if (requested)
            action.SetMappedKey(requested);

        Reset();
    }

    void ControlsScreenGui::InitImpl()
    {
        Environment::GetInput()->eventKeys.pressed.Subscribe(&ControlsScreenGui::OnInputPressed, *this);

        // Background
        auto background = Agui::Image::Factory(GetRoot(), "background", Agui::RelativePosition(Agui::HorizontalAlignment::MID, Agui::VerticalAlignment::MID), 0);
        background->GetSprite()->ScaleTo(1024, 768);
        background->GetSprite()->color.Edit(255, 100, 100, 255);
        Agui::CreateRootResFitter(*background);

        buttons.reserve(Environment::GetInput()->GetActions().size());
        // Left most column
        for (auto& loop : Environment::GetInput()->GetActions())
            AddButton(*loop);

        // Accept button
        auto button = Agui::PushButton::Factory(GetRoot(), "accept", Agui::RelativePosition(Agui::Dimension(0, 60), Agui::Dimension(0, -48), Agui::HorizontalAlignment::LEFT, Agui::VerticalAlignment::BOT), 1);
        button->GetText()->SetString("ACCEPT");
        button->GetText()->color = Agui::colorBlack;
        button->eventClicked.Subscribe(std::bind(&ControlsScreenGui::OnAcceptButtonClicked, this));

        // Cancel button
        button = Agui::PushButton::Factory(GetRoot(), "cancel", Agui::RelativePosition(Agui::Dimension(0, -60), Agui::Dimension(0, -48), Agui::HorizontalAlignment::RIGHT, Agui::VerticalAlignment::BOT), 1);
        button->GetText()->SetString("CANCEL");
        button->GetText()->color = Agui::colorBlack;
        button->eventClicked.Subscribe(std::bind(&ControlsScreenGui::OnCancelButtonClicked, this));
    }

    void ControlsScreenGui::SetButtonPressed(Button *setTo)
    {
        buttonPressed = setTo;
    }

    void ControlsScreenGui::OnInputPressed(const Input::Key &key)
    {
        if (buttonPressed && !Environment::GetInput()->IsKeyUsedForAction(key) && key.CanUseForAction())
        {
            buttonPressed->Request(key);
            buttonPressed = nullptr;
        }
    }

    void ControlsScreenGui::OnAcceptButtonClicked()
    {
        for (auto& loop : buttons)
            loop.LockIn();

        Environment::GetIni().Save();
        StateManager::Pop();
    }

    void ControlsScreenGui::OnCancelButtonClicked()
    {
        for (auto& loop : buttons)
            loop.Reset();

        StateManager::Pop();
    }

    void ControlsScreenGui::AddButton(Input::Action &action)
    {
        static const float X_START = 30;
        static const float Y_START = 30;
        static const float Y_INCREMENT = 60;
        static const float Y_THRESHOLD = 510;

        static const float Y_LABEL_DIFFERENCE = -20;

        static auto xTotal = X_START;
        static auto yTotal = Y_START;
        static Agui::HorizontalAlignment horizAlign(Agui::HorizontalAlignment::LEFT);
        static unsigned short count = 1;

        // Create push button and new ControlsButton
        auto pushButton = Agui::PushButton::Factory(GetRoot(), "button" + ToString(count), Agui::RelativePosition(Agui::Dimension(0, xTotal), Agui::Dimension(0, yTotal), horizAlign, Agui::VerticalAlignment::TOP), 1);
        buttons.push_back(Button(*pushButton, action));

        // Setup push button
        pushButton->ModifySprite(Agui::Sprite("buttonSmall.png", 1, Agui::Color()));
        pushButton->GetText()->SetString(action.GetMappedKey()->displayName);
        pushButton->GetText()->color = Agui::colorBlack;
        pushButton->eventClicked.Subscribe(std::bind(&ControlsScreenGui::SetButtonPressed, this, &buttons.back()));

        // Label
        auto label = Agui::Label::Factory(pushButton, "label", Agui::RelativePosition(Agui::Dimension(), Agui::Dimension(0, Y_LABEL_DIFFERENCE)), 0);
        label->ModifyText(Agui::Text(action.displayName + ":", Agui::Text::WORD_BREAK, *Agui::fontSlender, Agui::colorBlack));
        label->GetText()->SetAutoCalcTextSize();

        // Handle x and y positions
        yTotal += Y_INCREMENT;
        if (yTotal > Y_THRESHOLD)
        {
            yTotal = Y_START;

            if (horizAlign == Agui::HorizontalAlignment::LEFT)
            {
                horizAlign = Agui::HorizontalAlignment::MID;
                xTotal = 0;
            }
            else if (horizAlign == Agui::HorizontalAlignment::MID)
            {
                horizAlign = Agui::HorizontalAlignment::RIGHT;
                xTotal = X_START;
            }
        }

        ++count;
    }

    ControlsScreenGui::ControlsScreenGui() : StateGui("controlsScreen"), buttonPressed(nullptr)
    {}

    ControlsScreen controlsScreenState;
}