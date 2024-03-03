#pragma once

#include "BattleCharacter.h"
#include "InputAction.h"

namespace Atmos
{
    /*
    namespace Battle
    {
        class ActionManager
        {
        private:
            virtual void StartImpl() = 0;
            virtual void StopImpl() = 0;
            virtual bool CanStartImpl() const = 0;
            virtual void OnDeselectEntityImpl() = 0;

            virtual void OnKeyPressedImpl(const Input::Key &arg);
            virtual void OnActionActiveImpl(const Input::Action &arg);
            virtual void OnActionPressedImpl(const Input::Action &arg);
            virtual void OnMouseKeyPressedImpl(const Input::MouseKey &arg);
        protected:
            void AddPlayer(CharacterEntity &add, const GridPosition &position);
            CharacterEntity GetSelectedEntity() const;
            CharacterEntity FindCharacter(const GridPosition &position) const;
            void DeselectEntity();
            void SelectedEntityEndTurn();
            bool IsSelectedEntityValid() const;
            bool IsSelectedEntityPlayer() const;
            bool IsSelectedEntityMonster() const;
            bool HasSelectedEntityTurnEnded() const;

            // Call this when the placement is done - will setup everything and stop the placement
            void StartBattle();

            bool IsSelected() const;
            void ThisIsDone();
        public:
            void Start();
            void Stop();
            bool CanStart() const;
            void OnDeselectEntity();

            void OnKeyPressed(const Input::Key &arg);
            void OnActionActive(const Input::Action &arg);
            void OnActionPressed(const Input::Action &arg);
            void OnMouseKeyPressed(const Input::MouseKey &arg);
        };
    }
    */
}