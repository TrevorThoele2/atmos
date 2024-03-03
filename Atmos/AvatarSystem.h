#pragma once

#include "EntitySystem.h"
#include "EntityComponentPackage.h"

#include "AvatarComponent.h"
#include "CharacterComponent.h"
#include "InventoryComponent.h"
#include "CombatComponent.h"
#include "MovementComponent.h"
#include "SenseComponent.h"

#include "InputAction.h"

namespace Atmos
{
    namespace Ent
    {
        class ActionComponent;
        class AvatarSystem : public System<AvatarSystem>
        {
        public:
            enum ID
            {
                General,
                Avatar,
                Character,
                Inventory,
                Movement,
                Sense
            };
        private:
            typedef ComponentPackageRequired<
                Ent::GeneralComponent*,
                Ent::AvatarComponent*,
                Ent::CharacterComponent*,
                Ent::InventoryComponent*,
                Ent::MovementComponent*,
                Ent::SenseComponent*>
                Package;

            typedef System<AvatarSystem> SystemBaseT;
            friend SystemBaseT;

            Package package;
            Entity battleMonster;

            AvatarSystem();
            AvatarSystem(const AvatarSystem &arg) = delete;
            AvatarSystem& operator=(const AvatarSystem &arg) = delete;

            void Init() override;

            void OnActionPressed(const Input::Action &action);
            void OnActionActive(const Input::Action &action);
            void OnUsePressed();
            void OnFieldChanged(Field &field);
            void OnFinalizeField();

            void ExecuteActions(void(ActionComponent::*function)(), const GridPosition &pos);
            void Move(const Direction &direction);
            bool CanMove(const GridPosition &moveTo) const;
        protected:
            ApprovedStates GetApprovedStates() const override;
        public:
            static void SetEntity(Entity set);
            static Entity GetEntity();
            static bool IsValid();

            static Name& GetName();
            static GridPosition& GetPosition();
            static Direction& GetDirection();

            static GeneralComponent* GetGeneral();
            static AvatarComponent* GetAvatar();
            static CharacterComponent* GetCharacter();
            static InventoryComponent* GetInventory();
            static MovementComponent* GetMovement();
            static SenseComponent* GetSense();
        };
    }
}