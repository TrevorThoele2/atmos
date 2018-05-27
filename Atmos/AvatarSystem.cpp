
#include "AvatarSystem.h"

#include "EntityPositionSystem.h"
#include "ActionComponent.h"
#include "DialogueComponent.h"
#include "Speech.h"
#include "GameDialog.h"
#include "StatusScreen.h"
#include "WorldManager.h"
#include "Camera.h"
#include "StringUtility.h"

#include "Battle.h"
#include "MainGame.h"

#include "Environment.h"

#include "Logger.h"

namespace Atmos
{
    namespace Ent
    {
        AvatarSystem::AvatarSystem() : battleMonster(nullEntity)
        {}

        void AvatarSystem::Init()
        {
            SubscribeEvent(Environment::GetInput()->eventActionPressed, &AvatarSystem::OnActionPressed, *this);
            SubscribeEvent(Environment::GetInput()->eventActionActive, &AvatarSystem::OnActionActive, *this);
            SubscribeEvent(::Atmos::WorldManager::Instance().eventFieldSet, &AvatarSystem::OnFieldChanged, *this, ApprovedStates());
            SubscribeEvent(::Atmos::WorldManager::Instance().eventFinalizeField, &AvatarSystem::OnFinalizeField, *this, ApprovedStates());
        }

        void AvatarSystem::OnActionPressed(const Input::Action &action)
        {
            switch (action.id)
            {
            case Input::ActionID::USE:
                OnUsePressed();
                break;
            }
        }

        void AvatarSystem::OnActionActive(const Input::Action &action)
        {
            switch(action.id)
            {
            case Input::ActionID::MOVE_UP:
                if(!Environment::GetInput()->IsActionActive(Input::ActionID::MOVE_DOWN))
                    Move(Direction::UP);
                break;
            case Input::ActionID::MOVE_DOWN:
                if (!Environment::GetInput()->IsActionActive(Input::ActionID::MOVE_UP))
                    Move(Direction::DOWN);
                break;
            case Input::ActionID::MOVE_LEFT:
                if (!Environment::GetInput()->IsActionActive(Input::ActionID::MOVE_RIGHT))
                    Move(Direction::LEFT);
                break;
            case Input::ActionID::MOVE_RIGHT:
                if (!Environment::GetInput()->IsActionActive(Input::ActionID::MOVE_LEFT))
                    Move(Direction::RIGHT);
                break;
            }
        }

        void AvatarSystem::OnUsePressed()
        {
            // Find out if there is an entity with the dialogue in front
            {
                auto &entities = PositionSystem::FindEntities(GetGeneral()->GetPositionInFront());
                for (auto &loop : entities)
                {
                    auto found = GetCurrentEntities()->FindComponent<::Atmos::Ent::DialogueComponent>(loop);
                    if (found)
                    {
                        ::Atmos::Speech::Handler::Enter(*found);
                        return;
                    }
                }
            }

            // Attempt to pick up the items from the ground
            {
                auto &items = GetCurrentGroundItems()->PickUpItems(GetGeneral()->position);
                if (!items.empty())
                {
                    for (auto &loop : items)
                        GetInventory()->Add(std::move(loop));

                    return;
                }
            }

            // Execute actions for standing on top of the component
            ExecuteActions(&ActionComponent::OnUseOnTop, GetGeneral()->position);
        }

        void AvatarSystem::OnFieldChanged(Field &field)
        {
            auto &avatarMap = GetCurrentEntities()->GetRawMap<AvatarComponent>();
            if (avatarMap.empty())
            {
                Logger::Log("There was no avatar in the field.",
                    Logger::Type::ERROR_SEVERE,
                    Logger::NameValueVector{ NameValuePair("Field ID", ToString(field.GetID())) });
                return;
            }

            if (avatarMap.size() > 1)
                Logger::Log("There were more than one avatar components in the field.",
                    Logger::Type::ERROR_SEVERE,
                    Logger::NameValueVector{ NameValuePair("Field ID", ToString(field.GetID())) });

            Entity entity = avatarMap.begin()->second.GetOwnerEntity();
            if (entity == GetEntity())
                return;

            // Set entity and check if it was actually set
            // If not, then it didn't have the correct components
            SetEntity(entity);
            if (GetEntity() == nullEntity)
            {
                String hasString;
                auto &hasComponents = package.GetHas(entity);
                for (auto loop = hasComponents.begin(); loop != hasComponents.end(); ++loop)
                {
                    hasString.append((*loop)->GetTypeName());
                    if (loop != --hasComponents.end())
                        hasString.append(", ");
                }

                Logger::Log("The avatar did not have the right components.",
                    Logger::Type::ERROR_SEVERE,
                    Logger::NameValueVector{
                    NameValuePair("Field ID", ToString(field.GetID())),
                    NameValuePair("Needs", String("General, Avatar, Character, Inventory, Movement, Sense")),
                    NameValuePair("Has", hasString)});
                return;
            }

            if (battleState.IsTop())
                GetSense()->Hide();
            else
            {
                GetSense()->Show();
                Camera::SetFocus(&GetSense()->position);
            }
        }

        void AvatarSystem::OnFinalizeField()
        {
            if (battleMonster != nullEntity)
            {
                auto &position = GetCurrentEntities()->GetPosition(battleMonster).Get();
                GetCurrentEntities()->DestroyEntity(battleMonster);
                PositionSystem::MoveEntityInstant(GetEntity(), position);
                battleMonster = nullEntity;
            }

            package.SetEntity(nullEntity);
        }

        void AvatarSystem::ExecuteActions(void(ActionComponent::*function)(), const GridPosition &pos)
        {
            auto &entities = PositionSystem::FindEntities(pos);
            for (auto &loop : entities)
            {
                auto found = GetCurrentEntities()->FindComponent<ActionComponent>(loop);
                if (found)
                    (found->*function)();
            }
        }

        void AvatarSystem::Move(const Direction &direction)
        {
            if (!IsValid())
                return;

            auto &moveTo = GetGeneral()->position.FindPositionAdjacent(direction);
            if (!CanMove(moveTo))
                return;

            if (PositionSystem::MoveEntity(GetEntity(), direction))
                ExecuteActions(&ActionComponent::OnMovedInto, moveTo);
        }

        bool AvatarSystem::CanMove(const GridPosition &moveTo) const
        {
            // Check if the system is valid at all
            if (!IsValid())
                return false;

            // If cannot move entity, then don't
            if (!PositionSystem::CanMove(GetEntity()))
                return false;

            // Check if these dialogs are open
            if (GameDialog::IsActive() || Speech::Handler::IsWorking() || StatusScreen::IsActive())
                return false;

            // Check moving into monster
            {
                // If not moving into anything, leave
                auto &otherFind = PositionSystem::FindEntities(moveTo);
                if (otherFind.empty())
                    return true;

                Entity monsterEntity = nullEntity;
                // If not moving into a monster, leave
                {
                    bool leave = true;
                    for (auto &loop : otherFind)
                    {
                        auto charComp = GetCurrentEntities()->FindComponent<Ent::CharacterComponent>(loop);
                        if (charComp && charComp->IsMonster())
                        {
                            leave = false;
                            monsterEntity = loop;
                            break;
                        }
                    }

                    if (leave)
                        return true;
                }

                // This is a player moving into a monster
                // Check if can go to battle state
                if (!battleState.SetupFromMonster(monsterEntity))
                    return true;

                Instance().battleMonster = monsterEntity;
                battleState.Goto();
                return false;
            }
        }

        AvatarSystem::ApprovedStates AvatarSystem::GetApprovedStates() const
        {
            return ApprovedStates(true, false);
        }

        void AvatarSystem::SetEntity(Entity set)
        {
            Instance().package.SetEntity(set);
            //auto ret = Instance().package.SetEntity(set);
            //if (ret)
                //GetCurrentEntities()->SignalTransfer(set);
        }

        Entity AvatarSystem::GetEntity()
        {
            return Instance().package.GetEntity();
        }

        bool AvatarSystem::IsValid()
        {
            return Instance().package.IsOccupied();
        }

        Name& AvatarSystem::GetName()
        {
            return GetGeneral()->name;
        }

        GridPosition& AvatarSystem::GetPosition()
        {
            return GetGeneral()->position;
        }

        Direction& AvatarSystem::GetDirection()
        {
            return GetGeneral()->direction;
        }

        GeneralComponent* AvatarSystem::GetGeneral()
        {
            return Instance().package.GetAt<General>();
        }

        AvatarComponent* AvatarSystem::GetAvatar()
        {
            return Instance().package.GetAt<Avatar>();
        }

        CharacterComponent* AvatarSystem::GetCharacter()
        {
            return Instance().package.GetAt<Character>();
        }

        InventoryComponent* AvatarSystem::GetInventory()
        {
            return Instance().package.GetAt<Inventory>();
        }

        MovementComponent* AvatarSystem::GetMovement()
        {
            return Instance().package.GetAt<Movement>();
        }

        ::Atmos::Ent::SenseComponent* AvatarSystem::GetSense()
        {
            return Instance().package.GetAt<Sense>();
        }

        ENTITY_SYSTEM_FORCE_INSTANTIATION(AvatarSystem);
    }
}