
#include "MainMenuState.h"

#include "OptionsState.h"
#include "GameState.h"

#include "ObjectManager.h"
#include "StateSystem.h"
#include "WorldSystem.h"
#include "FileSystem.h"
#include "EngineSystem.h"
#include "AudioSystem.h"
#include "MusicSystem.h"

#include <AGUI/Image.h>
#include <AGUI/Label.h>
#include <AGUI/PushButton.h>

namespace Atmos
{
    MainMenuStateGui::MainMenuStateGui(ObjectManager& objectManager) :
        StateGui(objectManager, "mainMenu")
    {}

    void MainMenuStateGui::DoInitialize()
    {
        auto root = Root();

        // Background
        auto image = Agui::Image::Factory(
            root,
            "background",
            Agui::RelativePosition(
                Agui::HorizontalAlignment::MID,
                Agui::VerticalAlignment::MID),
            0);
        image->GetSprite()->ScaleTo(1024, 768);
        image->GetSprite()->color.Edit(127, 100, 100, 255);
        Agui::CreateRootResFitter(*image);

        AddButton("newGame", "NEW GAME", [this](const ::Agui::Object&)
        {
            OnNew();
        });
        AddButton("loadGame", "LOAD GAME", [this](const ::Agui::Object&)
        {
            OnLoad();
        });
        AddButton("options", "OPTIONS", [this](const ::Agui::Object&)
        {
            FindStateSystem()->Goto<OptionsState>();
        });
        AddButton("exit", "EXIT", [this](const ::Agui::Object&)
        {
            FindEngineSystem()->Get()->Exit();
        });
    }

    void MainMenuStateGui::AddButton(const std::string& name, const std::string& label, const Agui::Event<Agui::Object>::Callback& clickedFunction)
    {
        static const float startY = -80;
        static const float incrementY = 80;
        float totalY = startY;

        auto root = Root();

        auto button = Agui::PushButton::Factory(
            root,
            name,
            Agui::RelativePosition(
                Agui::Dimension(),
                Agui::Dimension(0, totalY),
                Agui::HorizontalAlignment::MID,
                Agui::VerticalAlignment::MID),
            1);
        button->GetText()->SetString(label);
        button->GetText()->color.Edit(0, 0, 0);
        button->eventClicked.Subscribe(clickedFunction);

        totalY += incrementY;
    }

    void MainMenuStateGui::OnNew()
    {
        FindWorldSystem()->Get()->StartNew();
        FindStateSystem()->Goto<GameState>();
    }

    void MainMenuStateGui::OnLoad()
    {
        FindWorldSystem()->Get()->UseStasis("Autosave.stasis");
    }

    StateSystem* MainMenuStateGui::FindStateSystem()
    {
        return objectManager->FindSystem<StateSystem>();
    }

    EngineSystem* MainMenuStateGui::FindEngineSystem()
    {
        return objectManager->FindSystem<EngineSystem>();
    }

    WorldSystem* MainMenuStateGui::FindWorldSystem()
    {
        return objectManager->FindSystem<WorldSystem>();
    }

    const FileName MainMenuState::mainMenuMusicName("mainmenu.ogg");

    void MainMenuState::AddMainMenuMusic()
    {
        auto musicData = FindAudioSystem()->Get()->CreateAudioData(MainMenuPath());
        music = Manager()->CreateObject<AudioAsset>(MainMenuPath(), std::move(musicData));
    }

    void MainMenuState::RemoveMainMenuMusic()
    {
        Manager()->DestroyObject(music);
    }

    FilePath MainMenuState::MainMenuPath() const
    {
        auto& mainMenuMusicPath = FindFileSystem()->Get()->ExePath();
        mainMenuMusicPath.Append("Sound").AppendSeparator();
        mainMenuMusicPath.SetFileName(mainMenuMusicName);
        return mainMenuMusicPath;
    }

    void MainMenuState::DoOnFocused()
    {
        AddMainMenuMusic();
        FindMusicSystem()->BeginPlaying(mainMenuMusicName);
    }

    void MainMenuState::DoOnUnfocused()
    {
        FindMusicSystem()->TerminateCurrent();
        RemoveMainMenuMusic();
    }

    const FileSystem* MainMenuState::FindFileSystem() const
    {
        return Manager()->FindSystem<FileSystem>();
    }

    WorldSystem* MainMenuState::FindWorldSystem()
    {
        return Manager()->FindSystem<WorldSystem>();
    }

    AudioSystem* MainMenuState::FindAudioSystem()
    {
        return Manager()->FindSystem<AudioSystem>();
    }

    MusicSystem* MainMenuState::FindMusicSystem()
    {
        return Manager()->FindSystem<MusicSystem>();
    }

    const ObjectTypeName ObjectTraits<MainMenuState>::typeName = "MainMenuState";
}

namespace Inscription
{
    OBJECT_INSCRIPTER_DEFINE_MEMBERS(::Atmos::MainMenuState)
    {

    }
}