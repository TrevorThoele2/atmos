
#include "MainMenu.h"

#include "MainGame.h"
#include "Options.h"
#include "WorldManager.h"
#include "Environment.h"
#include "GameEnvironment.h"

#include "ObjectManager.h"
#include "MusicSystem.h"
#include "CurrentField.h"

#include <AGUI\Image.h>
#include <AGUI\Label.h>
#include <AGUI\PushButton.h>

namespace Atmos
{
    MainMenuGui::MainMenuGui() : StateGui("mainMenu")
    {}

    void MainMenuGui::InitImpl()
    {
        // Background
        auto image = Agui::Image::Factory(GetRoot(), "background", Agui::RelativePosition(Agui::HorizontalAlignment::MID, Agui::VerticalAlignment::MID), 0);
        image->GetSprite()->ScaleTo(1024, 768);
        image->GetSprite()->color.Edit(127, 100, 100, 255);
        Agui::CreateRootResFitter(*image);

        AddButton("newGame", "NEW GAME", std::bind(&MainMenuGui::OnNew, *this));
        AddButton("loadGame", "LOAD GAME", std::bind(&MainMenuGui::OnLoad, *this));
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
        GameEnvironment::GetWorldManager().StartNew();
        mainGameState.Goto();
    }

    void MainMenuGui::OnLoad()
    {
        GameEnvironment::GetWorldManager().UseStasis("Autosave.stasis");
    }

    const FileName MainMenu::mainMenuMusicName("mainmenu.ogg");

    void MainMenu::AddMainMenuMusic()
    {
        auto musicData = Environment::GetAudio()->CreateAudioData(MainMenuPath());
        music = GetGlobalObjectManager()->CreateObject<AudioAsset>(MainMenuPath(), std::move(musicData));
    }

    void MainMenu::RemoveMainMenuMusic()
    {
        GetGlobalObjectManager()->DestroyObject(music);
    }

    FilePath MainMenu::MainMenuPath() const
    {
        auto &mainMenuMusicPath = Environment::GetFileSystem()->GetExePath();
        mainMenuMusicPath.Append("Sound" + Environment::GetFileSystem()->GetFileSeparator());
        mainMenuMusicPath.SetFileName(mainMenuMusicName);
        return mainMenuMusicPath;
    }

    void MainMenu::OnFocusedImpl()
    {
        GameEnvironment::GetWorldManager().Clear();

        AddMainMenuMusic();

        RetrieveMusicSystem()->BeginPlaying(mainMenuMusicName);
    }

    void MainMenu::OnUnfocusedImpl()
    {
        RetrieveMusicSystem()->TerminateCurrent();

        RemoveMainMenuMusic();
    }

    MusicSystem* MainMenu::RetrieveMusicSystem()
    {
        return GetLocalObjectManager()->FindSystem<MusicSystem>();
    }

    MainMenu mainMenu;
}