
#pragma once

#include "State.h"
#include "Input.h"

namespace Atmos
{
    class CancelMenuGui : public StateGui
    {
    private:
        void InitImpl() override;
    public:
        CancelMenuGui();
    };

    class CancelMenu : public State<CancelMenuGui>
    {
    private:
        void InitImpl() override;

        void OnKeyPressed(const Input::Key &args);
    public:
        CancelMenu() = default;
    };

    extern CancelMenu cancelMenuState;
}