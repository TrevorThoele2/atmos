#pragma once

#include <vector>
#include <memory>
#include "State.h"

#include "BattleActionManager.h"
#include "BattleTeamID.h"
#include "BattleSquadID.h"

#include "InputAction.h"
#include "Enum.h"

#include <Agui\Textbox.h>

namespace Atmos
{
    /*
    namespace Battle
    {
        class Gui
        {
        private:
            Agui::Textbox *characterInfo;
            Agui::TextComponent *name;
            Agui::TextComponent *health;
            Agui::TextComponent *mana;
        public:
            Gui();
        };
    }

    namespace Battle
    {
        class CharacterEntity;
        class State;
        class Gui : public StateGui
        {
        private:
            friend State;
        private:
            Agui::Textbox *characterInfo;
            Agui::TextComponent *name;
            Agui::TextComponent *health;
            Agui::TextComponent *mana;
            CharacterEntity *selected;

            Agui::Root *spellbookRoot;
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
        public:
            enum class TeamConnectionType
            {
                ALLIES,
                ENEMIES,
                NEUTRAL
            };
        private:
            enum ActionManagerType
            {
                PLAYER_PLACER,
                MOVEMENT,
                ATTACK,
                ABILITY
            };
        private:
            struct TeamConnection
            {
                TeamID from;
                TeamID to;
                TeamConnectionType type;
                TeamConnection(TeamID from, TeamID to, TeamConnectionType type);
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

            typedef std::list<CharacterEntity> CharacterList;
            CharacterList all;
            CharacterList::iterator selectedEntity;

            typedef std::list<CharacterList::iterator> CharacterReferenceList;
            typedef std::map<TeamID, CharacterReferenceList> Teams;
            Teams teams;
            Teams::iterator selectedTeam;

            typedef std::list<TeamConnection> TeamConnectionList;
            TeamConnectionList teamConnections;

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
            void OnFocusedImpl() override;

            // Will setup the players and monsters after being placed
            void Start();
            void SelectTeam(Teams::iterator select);
            void SelectFirstTeam();
            void SelectNextTeam();
            void Leave();

            void CheckDone();

            CharacterList::iterator FindEntityIterator(Entity entity);
            TeamConnectionList::iterator FindTeamConnection(TeamID from, TeamID to);

            // Selected entity 
            void SelectEntity(CharacterList::iterator select);
            void SelectEntity(Entity select);
            void DeselectEntity();
            void SelectedEntityEndTurn();
            bool HasSelectedEntity() const;
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

            void AttemptEndTurn();
            void AttemptPickUpItems();
        public:
            State();

            void SetupFromMonster(Entity from);
            bool CanSetupFromMonster(Entity from);
            // Reinstate from a stasis file
            void ReinstateFromStasis();

            void AddEntity(Entity entity, TeamID team, SquadID squad);
            void ConnectTeams(TeamID from, TeamID to, TeamConnectionType connection);
            void GivePlacement(const GridPosition &add);

            void CheckTeamDone();

            FieldID GetOriginalFieldID() const;
        };

        template<State::ActionManagerType managerT>
        void State::RequestManager()
        {
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
    */
}