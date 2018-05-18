
#include "CombatComponent.h"

#include "Battle.h"
#include "MainGame.h"
#include "AvatarSystem.h"

#include <Inscription\UnorderedMap.h>
#include <Inscription\Memory.h>

#include "Assert.h"

namespace Atmos
{
    namespace Ent
    {
        INSCRIPTION_SERIALIZE_FUNCTION_DEFINE(CombatComponent)
        {
            scribe(stats);
            scribe(resources);
            scribe(acumen);
            scribe(proficiencies);
            scribe(effectiveness);
            scribe(charClass);
            scribe(permanentStash);
            scribe(equipment);
            scribe(currentExp);
            scribe(expToNextLevel);
            scribe(level);
            scribe(movementRange);

            if (scribe.IsOutput())
            {
                inscription::ContainerSize size(spells.size());
                scribe(size);

                for (auto &loop : spells)
                    scribe.Save(loop.second);
            }
            else
            {
                inscription::ContainerSize size;
                scribe.Load(size);

                while (size-- > 0)
                {
                    RegistryObjectReference<Spell> spell;
                    scribe.Load(spell);

                    spells.emplace(spell->GetName(), spell);
                }

                if (charClass)
                {
                    stats.SetCharacterClass(*charClass);
                    resources.SetCharacterClass(*charClass);
                }
            }
        }

        CombatComponent::CombatComponent(CombatComponent &&arg) :
            charClass(arg.charClass),
            stats(std::move(arg.stats)),
            resources(std::move(arg.resources)),
            acumen(std::move(arg.acumen)),
            proficiencies(std::move(arg.proficiencies)),
            effectiveness(std::move(arg.effectiveness)),
            permanentStash(std::move(arg.permanentStash)),
            equipment(std::move(arg.equipment)),
            currentExp(arg.currentExp), expToNextLevel(arg.expToNextLevel), level(std::move(arg.level)),
            movementRange(arg.movementRange),
            spells(std::move(arg.spells))
        {}

        CombatComponent& CombatComponent::operator=(const CombatComponent &arg)
        {
            charClass = arg.charClass;
            stats = arg.stats;
            resources = arg.resources;
            acumen = arg.acumen;
            proficiencies = arg.proficiencies;
            effectiveness = arg.effectiveness;
            permanentStash = arg.permanentStash;
            equipment = arg.equipment;
            currentExp = arg.currentExp;
            expToNextLevel = arg.expToNextLevel;
            level = arg.level;
            movementRange = arg.movementRange;
            spells = std::move(arg.spells);
            return *this;
        }

        CombatComponent& CombatComponent::operator=(CombatComponent &&arg)
        {
            charClass = arg.charClass;
            stats = std::move(arg.stats);
            resources = std::move(arg.resources);
            acumen = std::move(arg.acumen);
            proficiencies = std::move(arg.proficiencies);
            effectiveness = std::move(arg.effectiveness);
            permanentStash = std::move(arg.permanentStash);
            equipment = std::move(arg.equipment);
            currentExp = arg.currentExp;
            expToNextLevel = arg.expToNextLevel;
            level = std::move(arg.level);
            movementRange = arg.movementRange;
            spells = std::move(arg.spells);
            return *this;
        }

        bool CombatComponent::operator==(const CombatComponent &arg) const
        {
            return charClass == arg.charClass && stats == arg.stats && resources == arg.resources && acumen == arg.acumen && proficiencies == arg.proficiencies &&
                effectiveness == arg.effectiveness && permanentStash == arg.permanentStash && equipment == arg.equipment && currentExp == arg.currentExp &&
                expToNextLevel == arg.expToNextLevel && level == arg.level && movementRange == arg.movementRange && spells == arg.spells;
        }

        bool CombatComponent::operator!=(const CombatComponent &arg) const
        {
            return !(*this == arg);
        }

        void CombatComponent::SetClass(const RegistryObjectReference<CharacterClass> &set)
        {
            ATMOS_ASSERT_MESSAGE(set.Get(), "This registry object MUST be set before-hand");

            ItemStashSize prevPermItemCount = charClass->permanentMaxItemCount;
            ItemStashSize prevTempItemCount = charClass->temporaryMaxItemCount;

            charClass = set;
            stats.SetCharacterClass(set);

            // Remove all items that are going to be "lost"
            // Permanent
            {
                auto avatarInventoryComponent = AvatarSystem::GetInventory();
                auto loop = permanentStash.rbegin();
                while (permanentStash.size() > charClass->permanentMaxItemCount)
                {
                    avatarInventoryComponent->Add((*loop)->GetName(), ItemStack::CountT(1));
                    permanentStash.erase(loop);
                }
            }
        }

        bool CombatComponent::IsCorpse() const
        {
            return resources.GetHealth() == 0;
        }

        bool CombatComponent::GiveItem(const RegistryObjectReference<Item> &item)
        {
            if (!HasSpaceInStash())
                return false;

            permanentStash.AddBack(item.Get());
            return true;
        }

        bool CombatComponent::RemoveItem(ItemStashSize pos)
        {
            if (permanentStash.empty() || permanentStash.size() - 1 < pos)
                return false;

            permanentStash.Remove(pos);
            return true;
        }

        RegistryObjectReference<Item> CombatComponent::FindItem(ItemStashSize pos)
        {
            auto found = permanentStash.Find(pos);
            if (found == permanentStash.end())
                return nullptr;

            return *found;
        }

        bool CombatComponent::HasSpaceInStash() const
        {
            return permanentStash.size() < charClass->permanentMaxItemCount;
        }

        void CombatComponent::Equip(EquipSlot slot, const RegistryObjectReference<Item> &item)
        {

        }

        void CombatComponent::Unequip(EquipSlot slot)
        {

        }

        RegistryObjectReference<Item> CombatComponent::GetEquipment(EquipSlot slot) const
        {
            auto found = equipment.find(slot);
            if (found == equipment.end())
                return RegistryObjectReference<Item>();

            return found->second;
        }

        bool CombatComponent::CanEquip(EquipSlot slot) const
        {
            return equipment.find(slot) == equipment.end();
        }

        bool CombatComponent::LearnSpell(const RegistryObjectReference<Spell> &learn)
        {
            if (!CanLearnSpell(learn))
                return false;

            spells.emplace(learn->GetName(), learn);
            return true;
        }

        bool CombatComponent::CanLearnSpell(const RegistryObjectReference<Spell> &test) const
        {
            if (!charClass)
                return false;

            return test->allowedClasses.In(*charClass);
        }

        Defense CombatComponent::GetDefense() const
        {
            Defense ret = 0;
            for (auto& loop : equipment)
            {
                ATMOS_ASSERT_MESSAGE(loop.second->equippableAspect, "Everything equipped must have an equippable aspect.");
                ret += loop.second->equippableAspect->defense;
            }

            return ret;
        }

        Element CombatComponent::GetElement() const
        {
            auto found = equipment.find(EquipSlot::SHARD);
            if (found == equipment.end())
                return GlobalContext<Element>::neutral;
            else
                return found->second->equippableAspect->AsShard()->element;
        }
    }
}