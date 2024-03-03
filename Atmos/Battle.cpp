
#include "Battle.h"

#include "BattleCharacter.h"
#include "BattleActionMovement.h"
#include "BattleActionAttack.h"
#include "BattleActionAbility.h"
#include "BattleActionPlacement.h"

#include "GeneralComponent.h"

#include "MainGame.h"
#include "GridSize.h"
#include "Direction.h"
#include "WorldManager.h"
#include "EntityAISystem.h"
#include "ActionSystem.h"
#include "ScriptController.h"
#include "StringUtility.h"

#include "Environment.h"
#include "GameEnvironment.h"

#include "FontDefines.h"

namespace Atmos
{
    namespace Battle
    {
        Gui::Gui()
        {
            characterInfo = Agui::Textbox::Factory(mainGameState.gui.GetRoot(), "charInfo", Agui::RelativePosition(Agui::HorizontalAlignment::MID, Agui::VerticalAlignment::TOP), 0);
            characterInfo->GetSprite()->color.Edit(150, 0, 255, 0);
            characterInfo->SetShowWithParent(false);
            characterInfo->GetLayout()->ChangeDimensions(768, 192);
            characterInfo->GetLayout()->SetSelfModifySize(false);

            name = &characterInfo->CreateText("name", 1, Agui::RelativePosition(), Agui::Text("", Agui::Text::Format(), *Agui::fontButton, Agui::Color(255, 0, 0, 0))).GetText();
            name->SetAutoCalcTextSize();

            health = &characterInfo->CreateText("health", 1, Agui::RelativePosition(), Agui::Text("", Agui::Text::Format(), *Agui::fontButton, Agui::Color(255, 0, 0, 0))).GetText();
            health->SetAutoCalcTextSize();

            mana = &characterInfo->CreateText("mana", 1, Agui::RelativePosition(), Agui::Text("", Agui::Text::Format(), *Agui::fontButton, Agui::Color(255, 0, 0, 0))).GetText();
            mana->SetAutoCalcTextSize();
        }
    }

    /*
    namespace Battle
    {
        void Gui::InitImpl()
        {
            characterInfo = Agui::Textbox::Factory(GetRoot(), "charInfo", Agui::RelativePosition(Agui::HorizontalAlignment::MID, Agui::VerticalAlignment::TOP), 0);
            characterInfo->GetSprite()->color.Edit(150, 0, 255, 0);
            characterInfo->SetShowWithParent(false);
            characterInfo->CreateLayout<Agui::SequenceLayout>(Agui::Orientation::VERTICAL, 2.5f);
            characterInfo->GetLayout()->ChangeDimensions(768, 192);
            characterInfo->GetLayout()->SetSelfModifySize(false);

            name = &characterInfo->CreateText("name", 1, Agui::RelativePosition(), Agui::Text("", Agui::Text::Format(), *Agui::fontButton, Agui::Color(255, 0, 0, 0))).GetText();
            name->SetAutoCalcTextSize();

            health = &characterInfo->CreateText("health", 1, Agui::RelativePosition(), Agui::Text("", Agui::Text::Format(), *Agui::fontButton, Agui::Color(255, 0, 0, 0))).GetText();
            health->SetAutoCalcTextSize();

            mana = &characterInfo->CreateText("mana", 1, Agui::RelativePosition(), Agui::Text("", Agui::Text::Format(), *Agui::fontButton, Agui::Color(255, 0, 0, 0))).GetText();
            mana->SetAutoCalcTextSize();

            InitSpells();
        }

        void Gui::UpdateImpl()
        {
            if (selected)
            {
                // Set character info
                name->SetString("Name: " + *(*selected)->GetName());
                health->SetString("Health: " + ToString((*selected)->GetAt<CharacterEntity::Combat>()->resources.GetHealth()) + " / " + ToString((*selected)->GetAt<CharacterEntity::Combat>()->resources.GetMaxHealth()));
                mana->SetString("Mana: " + ToString((*selected)->GetAt<CharacterEntity::Combat>()->resources.GetMana()) + " / " + ToString((*selected)->GetAt<CharacterEntity::Combat>()->resources.GetMaxMana()));
            }
        }

        void Gui::InitSpells()
        {
            spellbookRoot = Agui::Root::Factory(GetRoot(), "spellbookRoot");

            // Description
            auto description = Agui::SpellDescriptionBox::Factory(spellbookRoot, "description", Agui::RelativePosition(Agui::Dimension(0, 535), Agui::Dimension(0, 5)), 0);
            description->ScaleTo(224, 224);
            description->GetSprite()->color.Edit(255, 255, 100);

            // Context menu
            auto contextMenu = Agui::SpellContextMenu::Factory(spellbookRoot, "contextMenu", Agui::RelativePosition(Agui::Dimension(0, 535), Agui::Dimension(0, 254)), 0);
            contextMenu->GetMenu()->GetLayout()->SetSelfModifySize(false);
            contextMenu->GetMenu()->GetLayout()->ChangeDimensions(224, 224);
            contextMenu->GetMenu()->GetSprite()->color.Edit(255, 255, 100);
            contextMenu->SetShowWithParent(false);

            spellbook.Init(*spellbookRoot, Agui::RelativePosition(Agui::HorizontalAlignment::RIGHT, Agui::VerticalAlignment::TOP), *description, contextMenu);
        }

        void Gui::OnCharacterChanged(CharacterEntity *character)
        {
            selected = character;
            if (!character)
                characterInfo->Hide();
            else
                characterInfo->Show();

            if (IsSpellbookActive())
            {
                if (!selected || ((*selected)->FindOther<::Atmos::Ent::CharacterComponent>() && (*selected)->FindOther<::Atmos::Ent::CharacterComponent>()->IsMonster()))
                    StopSpellbook();
                else
                    spellbook.SetList((*selected)->GetAt<CharacterEntity::Combat>());
            }
        }

        void Gui::OnSpellSelected()
        {
            StopSpellbook();
        }

        void Gui::OnActionPressed(const Input::Action &arg)
        {
            switch (arg.id)
            {
            case Input::ActionID::CANCEL:
                StopSpellbook();
                break;
            }
        }

        void Gui::StartSpellbook()
        {
            if (!selected || (*selected)->GetAt<CharacterEntity::Character>()->type != Ent::CharacterComponent::Type::PLAYER)
                return;

            spellbookRoot->Show();
            spellbook.Start();
            spellbook.SetList((*selected)->GetAt<CharacterEntity::Combat>());
        }

        void Gui::StopSpellbook()
        {
            spellbookRoot->Hide();
            spellbook.Stop();
        }

        Gui::Gui() : StateGui("battle"), characterInfo(nullptr), name(nullptr), health(nullptr), mana(nullptr), selected(nullptr), spellbookRoot(nullptr)
        {}

        void Gui::ToggleSpellbook()
        {
            if (!IsSpellbookActive())
                StartSpellbook();
            else
                StopSpellbook();
        }

        bool Gui::IsSpellbookActive() const
        {
            return spellbook.IsActive();
        }

        State::TeamConnection::TeamConnection(TeamID from, TeamID to, TeamConnectionType type) : from(from), to(to), type(type)
        {}

        void State::OnKeyPressed(const Input::Key &arg)
        {
            if (arg.id == Input::KeyID::HOME)
                Leave();

            if (IsSelectedManagerValid())
                (*selectedManager)->OnKeyPressed(arg);
        }

        void State::OnActionActive(const Input::Action &arg)
        {
            if (IsSelectedManagerValid())
                (*selectedManager)->OnActionActive(arg);

            if (HasSelectedEntity())
                return;

            switch (arg.id)
            {
            case Input::ActionID::MOVE_UP:
                GameEnvironment::GetCamera().Move(Direction::ValueT::UP, cameraMovementSpeed);
                break;
            case Input::ActionID::MOVE_DOWN:
                GameEnvironment::GetCamera().Move(Direction::ValueT::DOWN, cameraMovementSpeed);
                break;
            case Input::ActionID::MOVE_LEFT:
                GameEnvironment::GetCamera().Move(Direction::ValueT::LEFT, cameraMovementSpeed);
                break;
            case Input::ActionID::MOVE_RIGHT:
                GameEnvironment::GetCamera().Move(Direction::ValueT::RIGHT, cameraMovementSpeed);
                break;
            }
        }

        void State::OnActionPressed(const Input::Action &arg)
        {
            DoOnSelectedManager(&ActionManager::OnActionPressed, arg);

            switch (arg.id)
            {
            case Input::ActionID::ATTACK:
                RequestManager<ATTACK>();
                break;
            case Input::ActionID::OPEN_SPELLS:
                gui.ToggleSpellbook();
                break;
            case Input::ActionID::END_TURN:
                AttemptEndTurn();
                break;
            case Input::ActionID::USE:
                AttemptPickUpItems();
                break;
            }

            gui.OnActionPressed(arg);
        }

        void State::OnMouseKeyPressed(const Input::MouseKey &arg)
        {
            DoOnSelectedManager(&ActionManager::OnMouseKeyPressed, arg);

            switch (arg.id)
            {
            case Input::MouseButton::LEFT:
                bool selected = false;
                auto &position = GridPosition::FromScreen(Environment::GetInput()->GetMousePosition(), 0, GameEnvironment::GetCamera().GetTopLeft());
                for (auto loop = all.begin(); loop != all.end(); ++loop)
                {
                    if (loop->GetPosition()->x == position.x && loop->GetPosition()->y == position.y)
                    {
                        SelectEntity(loop);
                        selected = true;
                        break;
                    }
                }

                if (!selected)
                    DeselectEntity();

                break;
            }
        }

        void State::OnMouseKeyDoublePressed(const Input::MouseKey &arg)
        {
            auto &moveTo = Environment::GetInput()->GetMousePositionInGameCoordinates();

            auto &modulator = GameEnvironment::GenerateModulator(Modulator::Description::Camera);

            auto &trackX = GameEnvironment::GenerateModulatorTrack(Modulator::Description::Camera, Modulator::Description::Track::PositionX);
            auto node = trackX->CreateNode();
            node->SetEndState(Modulator::Value(static_cast<std::int64_t>(moveTo.x)));
            node->SetTimeTaken(TimeValue(1, TimeValue::EpochT::SECONDS));
            modulator->AddTrack(std::move(trackX));

            auto &trackY = GameEnvironment::GenerateModulatorTrack(Modulator::Description::Camera, Modulator::Description::Track::PositionY);
            node = trackY->CreateNode();
            node->SetEndState(Modulator::Value(static_cast<std::int64_t>(moveTo.y)));
            node->SetTimeTaken(TimeValue(1, TimeValue::EpochT::SECONDS));
            modulator->AddTrack(std::move(trackY));

            modulator->Start(GameEnvironment::GetCamera());
        }

        void State::OnFieldChanged(Field &field)
        {
            worldManagerSubscriber.Unsubscribe();

            selectedEntity = all.end();
            GameEnvironment::GetCamera().MoveToInstant(field.tiles.GetCenterOfTiles());
        }

        void State::OnAbilitySelected(const AbilityBase &ability)
        {
            gui.OnSpellSelected();
            RequestManager<ABILITY>();
        }

        void State::InitImpl()
        {
            worldManagerSubscriber.Setup(WorldManager::Instance().eventFieldSet, &State::OnFieldChanged, *this);

            SubscribeEvent(Environment::GetInput()->eventKeys.pressed, &State::OnKeyPressed, *this);
            SubscribeEvent(Environment::GetInput()->eventActionActive, &State::OnActionActive, *this);
            SubscribeEvent(Environment::GetInput()->eventActionPressed, &State::OnActionPressed, *this);
            SubscribeEvent(Environment::GetInput()->eventMouseKeys.pressed, &State::OnMouseKeyPressed, *this);
            SubscribeEvent(Environment::GetInput()->eventMouseKeys.doublePressed, &State::OnMouseKeyDoublePressed, *this);
            SubscribeEvent(AbilityUsing::Instance().eventStarted, &State::OnAbilitySelected, *this);
        }

        void State::WorkImpl()
        {
            Ent::ActionSystem::Work();
            LockInManager();
            ScriptController::Work();
            ScriptLocatorManager::Work();
            WorldManager::Work();

            if (HasSelectedEntity())
            {
                auto aiComponent = (*selectedEntity)->FindOther<::Atmos::Ent::AIComponent>();
                if (aiComponent)
                    Ent::AISystem::Execute(*aiComponent);
                else
                {
                    if (selectedEntity->GetCharacter()->IsMonster())
                        selectedEntity->GetBattle()->EndTurn();
                }
            }

            GetCurrentScheduler()->Work();

            GameEnvironment::GetCamera().Work();
        }

        bool State::CanGotoImpl() const
        {
            return canGoto;
        }

        void State::OnFocusedImpl()
        {
            CurrentMusic::StopPlaying();
        }

        void State::Start()
        {
            SelectFirstTeam();
            for (auto& loop : all)
                loop.GetBattle()->StartTurn();
        }

        void State::SelectTeam(Teams::iterator select)
        {
            selectedTeam = select;
            for (auto& loop : selectedTeam->second)
                loop->GetBattle()->StartTurn();
        }

        void State::SelectFirstTeam()
        {
            SelectTeam(teams.begin());
        }

        void State::SelectNextTeam()
        {
            ++selectedTeam;
            if (selectedTeam == teams.end())
                SelectFirstTeam();
            else
                SelectTeam(selectedTeam);
        }

        void State::Leave()
        {
            canGoto = false;
            WorldManager::Request(originalFieldID);
            mainGameState.Goto();
        }

        void State::CheckDone()
        {
            {
                bool allDead = true;
                for (auto& loop : all)
                    if (!loop.GetCombat()->IsDead())
                        allDead = false;

                if (allDead)
                    Leave();
            }
        }

        State::CharacterList::iterator State::FindEntityIterator(Entity entity)
        {
            for (auto loop = all.begin(); loop != all.end(); ++loop)
                if (loop->GetEntity() == entity)
                    return loop;

            return all.end();
        }

        State::TeamConnectionList::iterator State::FindTeamConnection(TeamID from, TeamID to)
        {
            for (auto loop = teamConnections.begin(); loop != teamConnections.end(); ++loop)
            {
                if (loop->from != from && loop->from != to)
                {
                    continue;
                }

                if (loop->to != from && loop->to != to)
                {
                    continue;
                }

                return loop;
            }

            return teamConnections.end();
        }

        void State::SelectEntity(CharacterList::iterator select)
        {
            if (select == all.end())
            {
                DeselectEntity();
                return;
            }

            selectedEntity = select;

            GameEnvironment::GetCamera().SetFocus(&selectedEntity->GetSense()->position);

            if (selectedEntity->GetCharacter()->IsPlayer() && !selectedEntity->GetBattle()->turnEnded)
                RequestManager<MOVEMENT>();
            else
                ResetManager();

            gui.OnCharacterChanged(&*selectedEntity);
            gui.Show();
        }

        void State::SelectEntity(Entity select)
        {
            for (auto loop = all.begin(); loop != all.end(); ++loop)
            {
                if (loop->GetEntity() == select)
                {
                    SelectEntity(loop);
                    return;
                }
            }
        }

        void State::DeselectEntity()
        {
            if (selectedEntity == all.end())
                return;

            GameEnvironment::GetCamera().ResetFocus();
            if (selectedEntity != all.end())
                GameEnvironment::GetCamera().MoveToInstant(selectedEntity->GetSense()->position);

            selectedEntity = all.end();
            gui.OnCharacterChanged(nullptr);
            gui.Hide();

            DoOnSelectedManager(&ActionManager::OnDeselectEntity);
        }

        void State::SelectedEntityEndTurn()
        {
            if (selectedEntity == all.end())
                return;

            selectedEntity->GetBattle()->EndTurn();
        }

        bool State::HasSelectedEntity() const
        {
            return selectedEntity != all.end();
        }

        bool State::IsSelectedEntityPlayer() const
        {
            return HasSelectedEntity() && selectedEntity->GetCharacter()->IsPlayer();
        }

        bool State::IsSelectedEntityMonster() const
        {
            return HasSelectedEntity() && selectedEntity->GetCharacter()->IsNPC();
        }

        bool State::HasSelectedEntityTurnEnded() const
        {
            return HasSelectedEntity() && selectedEntity->GetBattle()->turnEnded;
        }

        bool State::IsSelectedManagerValid() const
        {
            return selectedManager != actionManagers.end();
        }

        void State::ResetManager()
        {
            if (!IsSelectedManagerValid())
                return;

            (*selectedManager)->Stop();
            selectedManager = actionManagers.end();
        }

        void State::LockInManager()
        {
            if (requestedManager == actionManagers.end())
                return;

            // Stop the currently selected manager
            if (selectedManager != actionManagers.end())
                (*selectedManager)->Stop();

            // Select and start the requested manager
            selectedManager = requestedManager;
            (*selectedManager)->Start();
            requestedManager = actionManagers.end();
        }

        void State::AttemptEndTurn()
        {
            if (IsSelectedEntityPlayer() && !selectedEntity->GetBattle()->turnEnded)
            {
                SelectedEntityEndTurn();
                ResetManager();
            }
        }

        void State::AttemptPickUpItems()
        {
            if (!IsSelectedEntityPlayer())
                return;

            auto& itemStack = GetCurrentGroundItems()->PickUpItems(selectedEntity->GetGeneral()->position);
            // If the items received is none, then leave (nothing happened)
            if (itemStack.empty())
                return;

            // Attempt to give the selected entity the item, if not then leave (nothing happened)
            if (!selectedEntity->GetBattle()->GiveTemporaryItem(*itemStack.begin()->GetWrapped(), selectedEntity->GetCombat()->charClass))
                return;

            // End the turn of the player
            SelectedEntityEndTurn();
        }

        State::State() : canGoto(false), cameraMovementSpeed(6.0f * GRID_SIZE<float>), currentSquad(0)
        {
            actionManagers.push_back(ActionManagerPtr(new ActionPlacement()));
            actionManagers.push_back(ActionManagerPtr(new ActionMovement()));
            actionManagers.push_back(ActionManagerPtr(new ActionAttack()));
            actionManagers.push_back(ActionManagerPtr(new ActionAbility()));
            selectedManager = actionManagers.end();
            requestedManager = actionManagers.end();
        }

        void State::SetupFromMonster(Entity from)
        {
            canGoto = false;
            if (!CanSetupFromMonster(from))
                return;

            worldManagerSubscriber.Subscribe();
            canGoto = true;
            RequestManager<PLAYER_PLACER>();
        }

        bool State::CanSetupFromMonster(Entity from)
        {
            auto characterComponent = GetCurrentEntities()->FindComponent<Ent::CharacterComponent>(from);
            if (!characterComponent || !characterComponent->IsMonster())
                return false;

            originalFieldID = WorldManager::GetCurrentField()->GetID();
            if (!WorldManager::Request(characterComponent->GetImpl<::Atmos::Ent::CharacterComponent::Type::MONSTER>()->fieldID))
                return false;

            return true;
        }

        void State::ReinstateFromStasis()
        {
            canGoto = true;
        }

        void State::AddEntity(Entity entity, TeamID team, SquadID squad)
        {
            auto foundEntity = FindEntityIterator(entity);
            if (foundEntity == all.end())
                return;

            auto &foundTeam = teams.find(team);
            if (foundTeam == teams.end())
                foundTeam = teams.emplace(team, CharacterReferenceList()).first;

            foundTeam->second.push_back(foundEntity);
        }

        void State::ConnectTeams(TeamID from, TeamID to, TeamConnectionType connection)
        {
            auto found = FindTeamConnection(from, to);
            if (found == teamConnections.end())
                teamConnections.push_back(TeamConnection(from, to, connection));
            else
                found->type = connection;
        }

        void State::GivePlacement(const GridPosition &add)
        {
            static_cast<ActionPlacement*>(GetManager<PLAYER_PLACER>()->get())->AddPosition(add);
        }

        void State::CheckTeamDone()
        {
            CheckDone();

            {
                bool hasEntityWithTurn = false;
                for (auto &entityLoop : selectedTeam->second)
                {
                    if (entityLoop->GetBattle()->IsTurnEnded())
                    {
                        hasEntityWithTurn = true;
                        break;
                    }
                }

                if (!hasEntityWithTurn)
                    SelectNextTeam();
            }
        }

        FieldID State::GetOriginalFieldID() const
        {
            return originalFieldID;
        }
    }

    Battle::State battleState;
    */
}