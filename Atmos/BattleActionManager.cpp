
#include "BattleActionManager.h"

#include "Battle.h"

namespace Atmos
{
    /*
    namespace Battle
    {
        void ActionManager::OnKeyPressedImpl(const Input::Key &arg)
        {}

        void ActionManager::OnActionActiveImpl(const Input::Action &arg)
        {}

        void ActionManager::OnActionPressedImpl(const Input::Action &arg)
        {}

        void ActionManager::OnMouseKeyPressedImpl(const Input::MouseKey &arg)
        {}

        void ActionManager::AddPlayer(CharacterEntity &add, const GridPosition &position)
        {
            battleState.all.push_back(add);
            ::Atmos::Ent::PositionSystem::MoveEntityInstant(add->GetEntity(), position);
            //add.GetSense()->Show();
        }

        CharacterEntity ActionManager::GetSelectedEntity() const
        {
            if (!battleState.HasSelectedEntity())
                return CharacterEntity(Ent::nullEntity);
            else
                return *battleState.selectedEntity;
        }

        CharacterEntity ActionManager::FindCharacter(const GridPosition &position) const
        {
            for (auto& loop : battleState.all)
            {
                if (loop->GetPosition() == position)
                    return loop;
            }

            return CharacterEntity();
        }

        void ActionManager::DeselectEntity()
        {
            battleState.DeselectEntity();
        }

        void ActionManager::SelectedEntityEndTurn()
        {
            battleState.SelectedEntityEndTurn();
        }

        bool ActionManager::IsSelectedEntityValid() const
        {
            return battleState.HasSelectedEntity();
        }

        bool ActionManager::IsSelectedEntityPlayer() const
        {
            return battleState.IsSelectedEntityPlayer();
        }

        bool ActionManager::IsSelectedEntityMonster() const
        {
            return battleState.IsSelectedEntityMonster();
        }

        bool ActionManager::HasSelectedEntityTurnEnded() const
        {
            return battleState.HasSelectedEntityTurnEnded();
        }

        void ActionManager::StartBattle()
        {
            battleState.Start();
            ThisIsDone();
        }

        bool ActionManager::IsSelected() const
        {
            return battleState.selectedManager != battleState.actionManagers.end() && battleState.selectedManager->get() == this;
        }

        void ActionManager::ThisIsDone()
        {
            if (battleState.selectedManager->get() != this)
                return;

            Stop();

            // Reset the battle state's selected manager
            // If there is a selected entity, then move back to the movement...
            if (!battleState.IsSelectedManager<State::MOVEMENT>() && battleState.selectedEntity != battleState.all.end() && !HasSelectedEntityTurnEnded())
                battleState.RequestManager<State::MOVEMENT>();
            else
                battleState.ResetManager();
        }

        void ActionManager::Start()
        {
            StartImpl();
        }

        void ActionManager::Stop()
        {
            StopImpl();
        }

        bool ActionManager::CanStart() const
        {
            return CanStartImpl();
        }

        void ActionManager::OnDeselectEntity()
        {
            OnDeselectEntityImpl();
        }

        void ActionManager::OnKeyPressed(const Input::Key &arg)
        {
            OnKeyPressedImpl(arg);
        }

        void ActionManager::OnActionActive(const Input::Action &arg)
        {
            OnActionActiveImpl(arg);
        }

        void ActionManager::OnActionPressed(const Input::Action &arg)
        {
            OnActionPressedImpl(arg);
        }

        void ActionManager::OnMouseKeyPressed(const Input::MouseKey &arg)
        {
            OnMouseKeyPressedImpl(arg);
        }
    }
    */
}