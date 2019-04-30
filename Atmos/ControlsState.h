
#pragma once

#include <vector>
#include <string>

#include "StateWithGui.h"
#include "StateGui.h"
#include <AGUI/PushButton.h>

namespace Atmos
{
    namespace Input
    {
        class SignalBase;
        class Action;
        class Key;
        class System;
    }

    class InitializationFileSystem;

    class ControlsStateGui : public StateGui
    {
    public:
        ControlsStateGui(ObjectManager& objectManager);
    private:
        class Button
        {
        public:
            Button(Agui::PushButton& widget, Input::Action& action);
            Button(const Button& arg) = default;
            Button& operator=(const Button& arg) = default;

            void Request(const Input::SignalBase& request);
            void Reset();
            void LockIn();
        private:
            Agui::PushButton& widget;
            const Input::SignalBase* requested;
            Input::Action& action;
        };
    private:
        Button* pressedButton;
        std::vector<Button> buttons;
    private:
        void DoInitialize() override;
        void SetButtonPressed(Button* setTo);

        void OnInputPressed(const Input::Key& key);
        void OnAcceptButtonClicked();
        void OnCancelButtonClicked();

        void AddButton(Input::Action& action);
    private:
        Input::System* FindInputSystem();
        StateSystem* FindStateSystem();
        InitializationFileSystem* FindInitializationFileSystem();
    };

    class ControlsState : public StateWithGui<ControlsStateGui>
    {};

    template<>
    struct ObjectTraits<ControlsState> : ObjectTraitsBase<ControlsState>
    {
        static const ObjectTypeName typeName;
        static constexpr ObjectTypeList<State> bases = {};
    };
}

namespace Inscription
{
    DECLARE_OBJECT_INSCRIPTER(::Atmos::ControlsState)
    {
    public:
        OBJECT_INSCRIPTER_DECLARE_MEMBERS;
    };
}