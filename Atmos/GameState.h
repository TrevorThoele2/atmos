#pragma once

#include "StateWithGui.h"
#include "StateGui.h"

#include "InputKey.h"

namespace Atmos::Input
{
    class Key;
    class Action;
}

namespace Atmos::State
{
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
}

namespace Atmos
{
    template<>
    struct ObjectTraits<State::GameState> : ObjectTraitsBase<State::GameState>
    {
        static const ObjectTypeName typeName;
        static constexpr ObjectTypeList<State::State> bases = {};
    };
}

namespace Inscription
{
    template<>
    struct TableData<::Atmos::State::GameState, BinaryArchive> :
        public ObjectTableDataBase<::Atmos::State::GameState, BinaryArchive>
    {};

    template<>
    class Scribe<::Atmos::State::GameState, BinaryArchive> :
        public ObjectScribe<::Atmos::State::GameState, BinaryArchive>
    {
    public:
        class Table : public TableBase
        {};
    };
}