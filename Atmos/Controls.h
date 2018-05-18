
#pragma once

#include <vector>
#include <string>

#include "State.h"
#include <AGUI\PushButton.h>

namespace Atmos
{
    namespace Input
    {
        class SignalBase;
        class Action;
        class Key;
    }

    class ControlsScreenGui : public StateGui
    {
    private:
        class Button
        {
        private:
            agui::PushButton &widget;
            const Input::SignalBase *requested;
            Input::Action &action;
        public:
            Button(agui::PushButton &widget, Input::Action &action);
            Button(const Button &arg) = default;
            Button& operator=(const Button &arg) = default;
            void Request(const Input::SignalBase &request);
            void Reset();
            void LockIn();
        };
    private:
        Button *buttonPressed;
        std::vector<Button> buttons;

        void InitImpl() override;
        void SetButtonPressed(Button *setTo);

        void OnInputPressed(const Input::Key &key);
        void OnAcceptButtonClicked();
        void OnCancelButtonClicked();

        void AddButton(Input::Action &action);
    public:
        ControlsScreenGui();
    };

    typedef State<ControlsScreenGui> ControlsScreen;
    extern ControlsScreen controlsScreenState;
}