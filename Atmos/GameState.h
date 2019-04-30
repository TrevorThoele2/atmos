
#pragma once

#include "StateWithGui.h"
#include "StateGui.h"

#include "InputKey.h"

#include <AGUI/Textbox.h>

namespace Atmos
{
    namespace Input
    {
        class Key;
        class Action;
    }

    class GameStateGui : public StateGui
    {
    public:
        GameStateGui(ObjectManager& objectManager);
    private:
        void DoInitialize() override;
    };

    class GameState : public StateWithGui<GameStateGui>
    {
    public:
        GameState(ObjectManager& manager);
        bool CanNewMenu() const;
    private:
        void OnKeyPressed(const Input::Key& args);
        void OnActionPressed(const Input::Action& args);

        void DoInitialize() override;
        void DoOnFocused() override;

        bool AnyTertiaryOpen() const;
    };

    template<>
    struct ObjectTraits<GameState> : ObjectTraitsBase<GameState>
    {
        static const ObjectTypeName typeName;
        static constexpr ObjectTypeList<State> bases = {};
    };
}

namespace Inscription
{
    DECLARE_OBJECT_INSCRIPTER(::Atmos::GameState)
    {
    public:
        OBJECT_INSCRIPTER_DECLARE_MEMBERS;
    };
}