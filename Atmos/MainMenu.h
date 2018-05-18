
#pragma once

#include "State.h"

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
        void OnFocusedImpl() override;
    };

    extern MainMenu mainMenu;
}