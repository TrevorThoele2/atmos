
#include "Battle.h"

#include "BattleCharacter.h"
#include "BattleActionMovement.h"
#include "BattleActionAttack.h"
#include "BattleActionAbility.h"
#include "BattleActionPlacement.h"

#include "GeneralComponent.h"
#include "PlayerParty.h"

#include "MainGame.h"
#include "Camera.h"
#include "GridSize.h"
#include "Direction.h"
#include "WorldManager.h"
#include "EntityAISystem.h"
#include "ScriptController.h"
#include "StringUtility.h"
#include "CurrentMusic.h"

#include "Environment.h"

#include "FontDefines.h"

#include <Affecter/SmoothstepType.h>

namespace Atmos
{
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

            if (IsSelectedEntityValid())
                return;

            switch (arg.id)
            {
            case Input::ActionID::MOVE_UP:
                Camera::Move(Direction::ValueT::UP, cameraMovementSpeed);
                break;
            case Input::ActionID::MOVE_DOWN:
                Camera::Move(Direction::ValueT::DOWN, cameraMovementSpeed);
                break;
            case Input::ActionID::MOVE_LEFT:
                Camera::Move(Direction::ValueT::LEFT, cameraMovementSpeed);
                break;
            case Input::ActionID::MOVE_RIGHT:
                Camera::Move(Direction::ValueT::RIGHT, cameraMovementSpeed);
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
                auto &position = GridPosition::FromScreen(Environment::GetInput()->GetMousePosition(), 0, Camera::GetTopLeft());
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
            Camera::MoveTo<::Affecter::SmoothstepType>(Environment::GetInput()->GetMousePositionInGameCoords(), FrameTimer(TimeValue(FixedPoint64(1))));
        }

        void State::OnFieldChanged(Field &field)
        {
            worldManagerSubscriber.Unsubscribe();

            // Add monsters to all and monsters
            for (auto &loop : field.entities.GetRawMap<Ent::CharacterComponent>())
            {
                if (!loop.second.IsMonster())
                    continue;

                field.entities.CreateComponent<Ent::BattleComponent>(loop.first);
                CharacterEntity made(loop.first);
                if (!made.IsOccupied())
                {
                    field.entities.DestroyComponent<Ent::BattleComponent>(loop.first);
                    continue;
                }

                all.push_back(std::move(made));
                monsters.push_back(--all.end());
            }

            selectedEntity = all.end();
            Camera::MoveToInstant(field.tiles.GetCenterOfTiles());
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
            LockInManager();

            if(turn == Turn::MONSTER)
                Ent::AISystem::Work();

            GetCurrentScheduler()->Work();
            GetCurrentOrphanScripts()->Work();

            if (turn == Turn::MONSTER)
            {
                for (auto &loop : monsters)
                {
                    auto ai = GetCurrentEntities()->FindComponent<Ent::AIComponent>(loop->GetEntity());
                    if (!ai || !ai->battleAI.IsValid())
                        loop->GetBattle()->EndTurn();
                }
            }

            Camera::Work();
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
            for (auto &loop : players)
                loop->GetBattle()->StartTurn();

            for (auto &loop : monsters)
            {
                loop->GetBattle()->StartTurn();
                loop->GetBattle()->EndTurn();
            }
        }

        void State::SetTurn(Turn set)
        {
            if (set == turn)
                return;

            CharacterTeamVector *start = nullptr;
            CharacterTeamVector *end = nullptr;

            if (turn == Turn::PLAYER)
            {
                start = &monsters;
                end = &players;
            }
            else
            {
                start = &players;
                end = &monsters;
            }

            turn = set;

            for (auto &loop : *end)
                loop->GetBattle()->EndTurn();

            for (auto &loop : *start)
                loop->GetBattle()->StartTurn();

            if (turn == Turn::PLAYER && IsSelectedEntityPlayer())
                RequestManager<MOVEMENT>();
        }

        void State::FlipTurn()
        {
            (turn == Turn::PLAYER) ? SetTurn(Turn::MONSTER) : SetTurn(Turn::PLAYER);
            ++currentSquad;
            if (currentSquad > Ent::PlayerParty::squadCount * 2 - 1)
                currentSquad = 0;
        }

        void State::Leave()
        {
            canGoto = false;
            WorldManager::Request(originalFieldID);
            mainGameState.Goto();
        }

        void State::CheckDone()
        {
            auto allCorpses = [](CharacterTeamVector &vector)
            {
                for (auto &loop : vector)
                {
                    auto &resources = loop->GetCombat()->resources;
                    if (resources.GetHealth() != 0)
                        return false;
                }

                return true;
            };

            if (allCorpses(players))
                Leave();
            else if (allCorpses(monsters))
                Leave();
        }

        void State::SelectEntity(CharacterList::iterator select)
        {
            if (select == all.end())
            {
                DeselectEntity();
                return;
            }

            selectedEntity = select;

            Camera::SetFocus(&selectedEntity->GetSense()->position);

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

            Camera::SetFocus(static_cast<Position2D*>(nullptr));
            if (selectedEntity != all.end())
                Camera::MoveToInstant(selectedEntity->GetSense()->position);

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

        bool State::IsSelectedEntityValid() const
        {
            return selectedEntity != all.end();
        }

        bool State::IsSelectedEntityPlayer() const
        {
            return IsSelectedEntityValid() && selectedEntity->GetCharacter()->IsPlayer();
        }

        bool State::IsSelectedEntityMonster() const
        {
            return IsSelectedEntityValid() && selectedEntity->GetCharacter()->IsNPC();
        }

        bool State::HasSelectedEntityTurnEnded() const
        {
            return IsSelectedEntityValid() && selectedEntity->GetBattle()->turnEnded;
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

            if (turn == Turn::MONSTER)
            {
                (*selectedManager)->Stop();
                requestedManager = actionManagers.end();
                selectedManager = actionManagers.end();
            }

            // Stop the currently selected manager
            if (selectedManager != actionManagers.end())
                (*selectedManager)->Stop();

            // Select and start the requested manager
            selectedManager = requestedManager;
            (*selectedManager)->Start();
            requestedManager = actionManagers.end();
        }

        Entity State::FindClosestImpl(const GridPosition &pos, const CharacterTeamVector &vector) const
        {
            if (vector.empty())
                return Ent::nullEntity;

            Entity closestEntity = (*vector.begin())->GetEntity();
            GridPosition closestPos = (*vector.begin())->GetPosition().Get();
            auto closestDistance = closestPos.FindDistance(pos);
            for (auto &loop : vector)
            {
                auto &checkPos = loop->GetPosition().Get();
                auto distance = checkPos.FindDistance(pos);
                if (distance < closestDistance)
                {
                    closestEntity = loop->GetEntity();
                    closestPos = checkPos;
                    closestDistance = distance;
                }
            }

            return closestEntity;
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

        bool State::SetupFromMonster(Entity set)
        {
            canGoto = false;

            auto characterComponent = GetCurrentEntities()->FindComponent<Ent::CharacterComponent>(set);
            if (!characterComponent || !characterComponent->IsMonster())
                return false;

            originalFieldID = WorldManager::GetCurrentField()->GetID();
            if (!WorldManager::Request(characterComponent->GetImpl<::Atmos::Ent::CharacterComponent::Type::MONSTER>()->fieldID))
                return false;

            worldManagerSubscriber.Subscribe();
            canGoto = true;
            RequestManager<PLAYER_PLACER>();
            return true;
        }

        void State::ReinstateFromStasis()
        {
            canGoto = true;
        }

        void State::GivePlacement(const GridPosition &add)
        {
            static_cast<ActionPlacement*>(GetManager<PLAYER_PLACER>()->get())->AddPosition(add);
        }

        void State::CheckTurnEnded()
        {
            CheckDone();

            auto checkImpl = [](CharacterTeamVector &vector)
            {
                for (auto &loop : vector)
                {
                    if (!loop->GetBattle()->turnEnded)
                        return false;
                }
                
                return true;
            };

            bool check = false;
            if (turn == Turn::PLAYER)
                check = checkImpl(players);
            else if (turn == Turn::MONSTER)
                check = checkImpl(monsters);

            if (check)
                FlipTurn();
        }

        FieldID State::GetOriginalFieldID() const
        {
            return originalFieldID;
        }

        Entity State::FindClosestPlayer(const GridPosition &pos) const
        {
            return FindClosestImpl(pos, players);
        }

        Entity State::FindClosestMonster(const GridPosition &pos) const
        {
            return FindClosestImpl(pos, monsters);
        }
    }

    Battle::State battleState;
}