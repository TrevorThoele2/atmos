#pragma once

#include <vector>
#include <memory>
#include "State.h"

#include "BattleActionManager.h"

#include "InputAction.h"
#include "Enum.h"
#include "PlayerParty.h"

#include "Spellbook.h"
#include <Agui\Textbox.h>

namespace Atmos
{
    namespace Battle
    {
        class CharacterEntity;
        class State;
        class Gui : public StateGui
        {
        private:
            friend State;
        private:
            agui::Textbox *characterInfo;
            agui::TextComponent *name;
            agui::TextComponent *health;
            agui::TextComponent *mana;
            CharacterEntity *selected;

            agui::Root *spellbookRoot;
            Spellbook spellbook;

            void InitImpl() override;
            void UpdateImpl() override;
            void InitSpells();

            void OnCharacterChanged(CharacterEntity *character);
            void OnSpellSelected();
            void OnActionPressed(const Input::Action &arg);

            void StartSpellbook();
            void StopSpellbook();
        public:
            Gui();

            void ToggleSpellbook();
            bool IsSpellbookActive() const;
        };

        class State : public ::Atmos::State<Gui>
        {
        private:
            enum class Turn
            {
                PLAYER,
                MONSTER
            };

            enum ActionManagerType
            {
                PLAYER_PLACER,
                MOVEMENT,
                ATTACK,
                ABILITY
            };
        private:
            friend ActionManager;

            bool canGoto;
            FieldID originalFieldID;

            const float cameraMovementSpeed;

            // Action managers
            typedef std::unique_ptr<ActionManager> ActionManagerPtr;
            typedef std::vector<ActionManagerPtr> ActionManagerVector;
            ActionManagerVector actionManagers;
            ActionManagerVector::iterator selectedManager;
            ActionManagerVector::iterator requestedManager;

            Ent::PlayerParty::SizeT currentSquad;
            Turn turn;

            typedef std::list<CharacterEntity> CharacterList;
            CharacterList all;
            CharacterList::iterator selectedEntity;

            typedef std::vector<CharacterList::iterator> CharacterTeamVector;
            CharacterTeamVector players;
            CharacterTeamVector monsters;

            EventBoundSubscriber worldManagerSubscriber;

            void OnKeyPressed(const Input::Key &arg);
            void OnActionActive(const Input::Action &arg);
            void OnActionPressed(const Input::Action &arg);
            void OnMouseKeyPressed(const Input::MouseKey &arg);
            void OnMouseKeyDoublePressed(const Input::MouseKey &arg);
            void OnFieldChanged(Field &field);
            void OnAbilitySelected(const AbilityBase &ability);

            void InitImpl() override;
            void WorkImpl() override;
            bool CanGotoImpl() const override;

            // Will setup the players and monsters after being placed
            void Start();
            void SetTurn(Turn set);
            void FlipTurn();
            void Leave();

            void CheckDone();

            // Selected entity 
            void SelectEntity(CharacterList::iterator select);
            void SelectEntity(Entity select);
            void DeselectEntity();
            void SelectedEntityEndTurn();
            bool IsSelectedEntityValid() const;
            bool IsSelectedEntityPlayer() const;
            bool IsSelectedEntityMonster() const;
            bool HasSelectedEntityTurnEnded() const;

            // Selected manager
            bool IsSelectedManagerValid() const;
            template<ActionManagerType managerT>
            void RequestManager();
            template<ActionManagerType managerT>
            ActionManagerVector::iterator GetManager();
            template<ActionManagerType managerT>
            ActionManagerVector::const_iterator GetManager() const;
            void ResetManager();
            template<ActionManagerType managerT>
            bool IsSelectedManager() const;
            template<class Ret, class... Args>
            void DoOnSelectedManager(Ret(ActionManager::*func)(Args...), Args && ... args);
            void LockInManager();

            Entity FindClosestImpl(const GridPosition &pos, const CharacterTeamVector &vector) const;

            void AttemptEndTurn();
            void AttemptPickUpItems();
        public:
            State();

            // Returns if the monster was set correctly
            bool SetupFromMonster(Entity set);
            // Reinstate from a stasis file
            void ReinstateFromStasis();

            void GivePlacement(const GridPosition &add);

            void CheckTurnEnded();

            FieldID GetOriginalFieldID() const;

            Entity FindClosestPlayer(const GridPosition &pos) const;
            Entity FindClosestMonster(const GridPosition &pos) const;
        };

        template<State::ActionManagerType managerT>
        void State::RequestManager()
        {
            if (turn == Turn::MONSTER)
                return;

            requestedManager = GetManager<managerT>();
        }

        template<State::ActionManagerType managerT>
        typename State::ActionManagerVector::iterator State::GetManager()
        {
            return actionManagers.begin() + ConvertToUnderlyingType(managerT);
        }

        template<State::ActionManagerType managerT>
        typename State::ActionManagerVector::const_iterator State::GetManager() const
        {
            return actionManagers.begin() + ConvertToUnderlyingType(managerT);
        }

        template<State::ActionManagerType managerT>
        bool State::IsSelectedManager() const
        {
            if (!IsSelectedManagerValid())
                return false;

            return (selectedManager - actionManagers.begin()) == managerT;
        }

        template<class Ret, class... Args>
        void State::DoOnSelectedManager(Ret(ActionManager::*func)(Args...), Args && ... args)
        {
            if (!IsSelectedManagerValid())
                return;

            ((**selectedManager).*func)(std::forward<Args>(args)...);
        }
    }

    extern Battle::State battleState;
}