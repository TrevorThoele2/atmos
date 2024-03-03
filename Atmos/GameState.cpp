#include "GameState.h"

#include "StateSystem.h"
#include "InputSystem.h"

namespace Atmos
{
    GameStateGui::GameStateGui(ObjectManager& objectManager) : StateGui(objectManager, "mainGame")
    {}

    void GameStateGui::DoInitialize()
    {}

    GameState::GameState(ObjectManager& manager) : StateWithGui(manager)
    {}

    bool GameState::CanNewMenu() const
    {
        return IsTop() && !AnyTertiaryOpen();
    }

    void GameState::OnKeyPressed(const Input::Key& args)
    {
        if (args.id == Input::KeyID::ESCAPE)
        {
            if (CanNewMenu())
            {

            }

            return;
        }
    }

    void GameState::OnActionPressed(const Input::Action& args)
    {
        if (!CanNewMenu())
        {
            //StatusScreen::OnActionPressed(args);
            return;
        }

        switch (args.id)
        {
        case Input::ActionID::INVENTORY:
            //if (CanNewMenu())
                //StatusScreen::Goto(StatusScreen::PageID::INVENTORY);
            break;
        case Input::ActionID::OPEN_SPELLS:
            //if (CanNewMenu())
                //StatusScreen::Goto(StatusScreen::PageID::SPELLS);
            break;
        case Input::ActionID::STATS:
            //if (CanNewMenu())
                //StatusScreen::Goto(StatusScreen::PageID::STATS);
            break;
        }
    }

    void GameState::DoInitialize()
    {
        auto inputSystem = Manager()->FindSystem<Input::System>();
        ExecuteWhenTop(inputSystem->Get()->eventKeys.pressed, &GameState::OnKeyPressed, *this);
        ExecuteWhenTop(inputSystem->Get()->eventActionPressed, &GameState::OnActionPressed, *this);
    }

    void GameState::DoOnFocused()
    {}

    bool GameState::AnyTertiaryOpen() const
    {
        return false;
    }

    const ObjectTypeName ObjectTraits<GameState>::typeName = "GameState";
}