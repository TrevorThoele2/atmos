
#include "MainMenu.h"

#include "MainGame.h"
#include "Options.h"
#include "WorldManager.h"
#include "Environment.h"
#include "CurrentMusic.h"
#include "AudioRegistry.h"

#include <AGUI\Image.h>
#include <AGUI\Label.h>
#include <AGUI\PushButton.h>

namespace Atmos
{
    void MainMenuGui::InitImpl()
    {
        // Background
        auto image = Agui::Image::Factory(GetRoot(), "background", Agui::RelativePosition(Agui::HorizontalAlignment::MID, Agui::VerticalAlignment::MID), 0);
        image->GetSprite()->ScaleTo(1024, 768);
        image->GetSprite()->color.Edit(127, 100, 100, 255);
        Agui::CreateRootResFitter(*image);

        AddButton("newGame", "NEW GAME", std::bind(&MainMenuGui::OnNew, *this));
        //AddButton("loadGame", "LOAD GAME", std::bind(&FileHandler::Load));
        AddButton("options", "OPTIONS", std::bind(&OptionsScreen::Goto, &optionsScreen));
        AddButton("exit", "EXIT", std::bind(&Environment::Exit));

        // Version label
        auto label = Agui::Label::Factory(GetRoot(), "version", Agui::RelativePosition(Agui::Dimension(0, 10), Agui::Dimension(0, -10), Agui::HorizontalAlignment::LEFT, Agui::VerticalAlignment::BOT), 1);
        label->GetText()->SetAutoCalcTextSize();
        label->GetText()->color.Edit(0, 0, 0);
        label->GetText()->SetString(Environment::GetVersion());
    }

    void MainMenuGui::AddButton(const std::string &name, const std::string &str, const Agui::Event<Agui::Object>::Callback &func)
    {
        static const float Y_START = -80;
        static const float Y_INCREMENT = 80;
        static float Y_TOTAL = Y_START;

        auto button = Agui::PushButton::Factory(GetRoot(), name, Agui::RelativePosition(Agui::Dimension(), Agui::Dimension(0, Y_TOTAL), Agui::HorizontalAlignment::MID, Agui::VerticalAlignment::MID), 1);
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

    const FileName MainMenu::mainMenuMusic("mainmenu.ogg");

    void MainMenu::AddMainMenuMusic()
    {
        AudioRegistry::Register(GetMainMenuPath());
    }

    void MainMenu::RemoveMainMenuMusic()
    {
        AudioRegistry::Remove(mainMenuMusic);
    }

    FilePath MainMenu::GetMainMenuPath() const
    {
        auto &mainMenuMusicPath = Environment::GetFileSystem()->GetExePath();
        mainMenuMusicPath.Append("Sound" + Environment::GetFileSystem()->GetFileSeparator());
        mainMenuMusicPath.SetName(mainMenuMusic);
        return mainMenuMusicPath;
    }

    void MainMenu::OnFocusedImpl()
    {
        WorldManager::Clear();

        AddMainMenuMusic();

        CurrentMusic::ChangePlaying(mainMenuMusic);
    }

    void MainMenu::OnUnfocusedImpl()
    {
        CurrentMusic::StopPlaying();

        RemoveMainMenuMusic();
    }

    MainMenu mainMenu;
}