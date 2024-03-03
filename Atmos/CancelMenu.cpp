
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
        auto image = Agui::Image::Factory(root, "background", Agui::RelativePosition(Agui::Dimension(0.5, 0), Agui::Dimension(0.5, 0)), 0);
        image->GetSprite()->color.Edit(127, 100, 100, 255);
        Agui::CreateRootResFitter(*image);

        // Resume button
        auto button = Agui::PushButton::Factory(root, "resume", Agui::RelativePosition(Agui::Dimension(), Agui::Dimension(0, -80), Agui::HorizontalAlignment::MID, Agui::VerticalAlignment::MID), 1);
        button->GetText()->SetString("RESUME GAME");
        button->GetText()->color.Edit(0, 0, 0);
        button->eventClicked.Subscribe(std::bind(&StateManager::Pop));

        // Save game button
        button = Agui::PushButton::Factory(root, "saveGame", Agui::RelativePosition(Agui::HorizontalAlignment::MID, Agui::VerticalAlignment::MID), 1);
        button->GetText()->SetString("SAVE GAME");
        button->GetText()->color.Edit(0, 0, 0);
        //button->eventClicked.Subscribe(std::bind(&FileHandler::Save));

        // Goto options
        button = Agui::PushButton::Factory(root, "controls", Agui::RelativePosition(Agui::Dimension(), Agui::Dimension(0, 80), Agui::HorizontalAlignment::MID, Agui::VerticalAlignment::MID), 1);
        button->GetText()->SetString("OPTIONS");
        button->GetText()->color.Edit(0, 0, 0);
        button->eventClicked.Subscribe(std::bind(&OptionsScreen::Goto, &optionsScreen));

        // Exit game button
        button = Agui::PushButton::Factory(root, "exit", Agui::RelativePosition(Agui::Dimension(), Agui::Dimension(0, 160), Agui::HorizontalAlignment::MID, Agui::VerticalAlignment::MID), 1);
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