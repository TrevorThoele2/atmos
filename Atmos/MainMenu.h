
#pragma once

#include "State.h"

#include "ObjectReference.h"
#include "AudioAsset.h"

#include "FilePath.h"

namespace Atmos
{
    class MainMenuGui : public StateGui
    {
    public:
        MainMenuGui();
    private:
        void InitImpl() override;
        void AddButton(const std::string &name, const std::string &str, const Agui::Event<Agui::Object>::Callback &func);
        void OnNew();
        void OnLoad();
    };

    class ObjectManager;
    class MusicSystem;

    class MainMenu : public State<MainMenuGui>
    {
    private:
        static const FileName mainMenuMusicName;
    private:
        TypedObjectReference<AudioAsset> music;

        void AddMainMenuMusic();
        void RemoveMainMenuMusic();

        FilePath MainMenuPath() const;
    private:
        void OnFocusedImpl() override;
        void OnUnfocusedImpl() override;
    private:
        MusicSystem* RetrieveMusicSystem();
    };

    extern MainMenu mainMenu;
}