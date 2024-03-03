
#include "BattleComponent.h"

#include "Entity.h"
#include "CombatComponent.h"
#include "InventoryComponent.h"

#include "Weapon.h"

#include "Battle.h"

namespace Atmos
{
    namespace Ent
    {
        INSCRIPTION_SERIALIZE_FUNCTION_DEFINE(BattleComponent)
        {
            scribe(startingPos);
            scribe(movementRangeLeft);
            scribe(turnEnded);
            scribe(temporaryStash);
            scribe(statusEffects);
            scribe(acumen);
            scribe(isAlly);
        }

        BattleComponent::BattleComponent() : acumen(0), isAlly(false)
        {}

        BattleComponent::BattleComponent(BattleComponent &&arg) : startingPos(std::move(arg.startingPos)), movementRangeLeft(std::move(arg.movementRangeLeft)), turnEnded(std::move(arg.turnEnded)), temporaryStash(std::move(arg.temporaryStash)), statusEffects(std::move(arg.statusEffects)), acumen(std::move(arg.acumen)), isAlly(std::move(arg.isAlly))
        {}

        BattleComponent& BattleComponent::operator=(BattleComponent &&arg)
        {
            startingPos = std::move(arg.startingPos);
            movementRangeLeft = std::move(arg.movementRangeLeft);
            turnEnded = std::move(arg.turnEnded);
            temporaryStash = std::move(arg.temporaryStash);
            statusEffects = std::move(arg.statusEffects);
            acumen = std::move(arg.acumen);
            isAlly = std::move(arg.isAlly);
            return *this;
        }

        void BattleComponent::StartTurn()
        {
            turnEnded = false;
            startingPos = GetCurrentEntities()->GetPosition(GetOwnerEntity()).Get();
            movementRangeLeft = GetCurrentEntities()->FindComponent<CombatComponent>(GetOwnerEntity())->movementRange;
        }

        void BattleComponent::EndTurn()
        {
            if (turnEnded)
                return;

            turnEnded = true;

            auto combatComponent = GetCurrentEntities()->FindComponent<CombatComponent>(GetOwnerEntity());
            if (combatComponent)
                for (auto &loop : statusEffects)
                    loop.second.Affect(*combatComponent);

            battleState.CheckTurnEnded();
        }

        void BattleComponent::Attack(Entity target)
        {
            if (turnEnded)
                return;

            /*
            auto defaultAttack = [&](CombatComponent &source, CombatComponent &target)
            {
                DamageGenerator generator(DamageType::MELEE, ElementType::NEUTRAL, 50);
                generator.Generate(source).Affect(target);
                EndTurn();
            };
            */

            // The source MUST have a combat component
            auto sourceCombat = GetCurrentEntities()->FindComponent<CombatComponent>(GetOwnerEntity());
            if (!sourceCombat)
                return;

            // The target MUST have a combat component
            auto targetCombat = GetCurrentEntities()->FindComponent<CombatComponent>(target);
            if (!targetCombat)
                return;

            // If the source has an inventory
            /*
            auto sourceInventory = GetCurrentEntities()->FindComponent<InventoryComponent>(GetOwnerEntity());
            if (!sourceInventory)
            {
                defaultAttack(*sourceCombat, *targetCombat);
                return;
            }

            // The source MUST have a weapon equipped
            auto weapon = dynamic_cast<Weapon::Stack*>(sourceInventory->equipment.Find(EquipSlot::MAINHAND));
            if (!weapon)
            {
                defaultAttack(*sourceCombat, *targetCombat);
                return;
            }

            // Attack now
            weapon->Attack(GetOwnerEntity(), *targetCombat);
            */
        }

        bool BattleComponent::GiveTemporaryItem(const Item &item, const RegistryObjectReference<CharacterClass> &charClass)
        {
            if (!HasSpaceInTemporaryStash(charClass))
                return false;

            temporaryStash.AddBack(&item);
            return true;
        }

        bool BattleComponent::RemoveTemporaryItem(ItemStashSize pos, const RegistryObjectReference<CharacterClass> &charClass)
        {
            if (temporaryStash.empty() || temporaryStash.size() - 1 < pos)
                return false;

            temporaryStash.Remove(pos);
            return true;
        }

        bool BattleComponent::HasSpaceInTemporaryStash(const RegistryObjectReference<CharacterClass> &charClass) const
        {
            return temporaryStash.size() < charClass->temporaryMaxItemCount;
        }

        bool BattleComponent::IsAlly() const
        {
            return isAlly;
        }

        bool BattleComponent::IsEnemy() const
        {
            return isAlly == false;
        }
    }
}