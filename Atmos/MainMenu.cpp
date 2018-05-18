
#include "MainMenu.h"

#include "MainGame.h"
#include "Options.h"
#include "WorldManager.h"
#include "Environment.h"

#include <AGUI\Image.h>
#include <AGUI\Label.h>
#include <AGUI\PushButton.h>

namespace Atmos
{
    void MainMenuGui::InitImpl()
    {
        // Background
        auto image = agui::Image::Factory(GetRoot(), "background", agui::RelativePosition(agui::HorizontalAlignment::MID, agui::VerticalAlignment::MID), 0);
        image->GetSprite()->ScaleTo(1024, 768);
        image->GetSprite()->color.Edit(127, 100, 100, 255);
        agui::CreateRootResFitter(*image);

        AddButton("newGame", "NEW GAME", std::bind(&MainMenuGui::OnNew, *this));
        //AddButton("loadGame", "LOAD GAME", std::bind(&FileHandler::Load));
        AddButton("options", "OPTIONS", std::bind(&OptionsScreen::Goto, &optionsScreen));
        AddButton("exit", "EXIT", std::bind(&Environment::Exit));

        // Version label
        auto label = agui::Label::Factory(GetRoot(), "version", agui::RelativePosition(agui::Dimension(0, 10), agui::Dimension(0, -10), agui::HorizontalAlignment::LEFT, agui::VerticalAlignment::BOT), 1);
        label->GetText()->SetAutoCalcTextSize();
        label->GetText()->color.Edit(0, 0, 0);
        label->GetText()->SetString(Environment::GetVersion());
    }

    void MainMenuGui::AddButton(const std::string &name, const std::string &str, const agui::Event<agui::Object>::Callback &func)
    {
        static const float Y_START = -80;
        static const float Y_INCREMENT = 80;
        static float Y_TOTAL = Y_START;

        auto button = agui::PushButton::Factory(GetRoot(), name, agui::RelativePosition(agui::Dimension(), agui::Dimension(0, Y_TOTAL), agui::HorizontalAlignment::MID, agui::VerticalAlignment::MID), 1);
        button->GetText()->SetString(str);
        button->GetText()->color.Edit(0, 0, 0);
        button->eventClicked.Subscribe(func);

        Y_TOTAL += Y_INCREMENT;
    }

    void MainMenuGui::OnNew()
    {
        WorldManager::StartNew();
        mainGameState.Goto();
    }

    MainMenuGui::MainMenuGui() : StateGui("mainMenu")
    {}

    void MainMenu::OnFocusedImpl()
    {
        WorldManager::Clear();
    }

    MainMenu mainMenu;
}