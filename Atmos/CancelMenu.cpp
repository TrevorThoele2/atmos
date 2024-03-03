
#include "CancelMenu.h"

#include "Options.h"
#include "MainGame.h"
#include "MainMenu.h"
#include "StateManager.h"

#include "Environment.h"

#include <AGUI\PushButton.h>
#include <AGUI\Image.h>

namespace Atmos
{
    void CancelMenuGui::InitImpl()
    {
        auto root = GetRoot();

        // Background
        auto image = agui::Image::Factory(root, "background", agui::RelativePosition(agui::Dimension(0.5, 0), agui::Dimension(0.5, 0)), 0);
        image->GetSprite()->color.Edit(127, 100, 100, 255);
        agui::CreateRootResFitter(*image);

        // Resume button
        auto button = agui::PushButton::Factory(root, "resume", agui::RelativePosition(agui::Dimension(), agui::Dimension(0, -80), agui::HorizontalAlignment::MID, agui::VerticalAlignment::MID), 1);
        button->GetText()->SetString("RESUME GAME");
        button->GetText()->color.Edit(0, 0, 0);
        button->eventClicked.Subscribe(std::bind(&StateManager::Pop));

        // Save game button
        button = agui::PushButton::Factory(root, "saveGame", agui::RelativePosition(agui::HorizontalAlignment::MID, agui::VerticalAlignment::MID), 1);
        button->GetText()->SetString("SAVE GAME");
        button->GetText()->color.Edit(0, 0, 0);
        //button->eventClicked.Subscribe(std::bind(&FileHandler::Save));

        // Goto options
        button = agui::PushButton::Factory(root, "controls", agui::RelativePosition(agui::Dimension(), agui::Dimension(0, 80), agui::HorizontalAlignment::MID, agui::VerticalAlignment::MID), 1);
        button->GetText()->SetString("OPTIONS");
        button->GetText()->color.Edit(0, 0, 0);
        button->eventClicked.Subscribe(std::bind(&OptionsScreen::Goto, &optionsScreen));

        // Exit game button
        button = agui::PushButton::Factory(root, "exit", agui::RelativePosition(agui::Dimension(), agui::Dimension(0, 160), agui::HorizontalAlignment::MID, agui::VerticalAlignment::MID), 1);
        button->GetText()->SetString("EXIT");
        button->GetText()->color.Edit(0, 0, 0);
        button->eventClicked.Subscribe(std::bind(&MainMenu::Goto, &mainMenu));
    }

    CancelMenuGui::CancelMenuGui() : StateGui("cancelMenu")
    {}

    void CancelMenu::InitImpl()
    {
        SubscribeEvent(Environment::GetInput()->eventKeys.pressed, &CancelMenu::OnKeyPressed, *this);
    }

    void CancelMenu::OnKeyPressed(const Input::Key &args)
    {
        if (args.id == Input::KeyID::ESCAPE)
        {
            StateManager::Pop();
            return;
        }
    }

    CancelMenu cancelMenuState;
}