
#pragma once

#include "State.h"

#include "FilePath.h"

namespace Atmos
{
    class MainMenuGui : public StateGui
    {
    private:
        void InitImpl() override;
        void AddButton(const std::string &name, const std::string &str, const agui::Event<agui::Object>::Callback &func);
        void OnNew();
    public:
        MainMenuGui();
    };

    class MainMenu : public State<MainMenuGui>
    {
    private:
        static const FileName mainMenuMusic;;
    private:
        void AddMainMenuMusic();
        void RemoveMainMenuMusic();

        FilePath GetMainMenuPath() const;
    private:
        void OnFocusedImpl() override;
        void OnUnfocusedImpl() override;
    };

    extern MainMenu mainMenu;
}