
#pragma once

#include "State.h"
#include "Input.h"

#include <AGUI\Textbox.h>

namespace Atmos
{
    namespace Input
    {
        class Key;
        class Action;
    }

    class MainGameGui : public StateGui
    {
    private:
        void InitImpl() override;
    public:
        MainGameGui();
    };

    class MainGame : public State<MainGameGui>
    {
    private:
        void OnKeyPressed(const Input::Key &args);
        void OnActionPressed(const Input::Action &args);
        void UseOnClick();

        void InitImpl() override;
        void WorkImpl() override;
        bool AnyTertiaryOpen() const;
    public:
        MainGame() = default;
        bool CanNewMenu() const;
    };

    extern MainGame mainGameState;
}