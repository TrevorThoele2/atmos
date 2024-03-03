
#include "CombatComponent.h"

#include "Battle.h"
#include "MainGame.h"
#include "AvatarSystem.h"
#include "InventoryComponent.h"

#include <Inscription\UnorderedMap.h>
#include <Inscription\Memory.h>

#include "Assert.h"

namespace Atmos
{
    namespace Ent
    {
        nCombatComponent::nCombatComponent(EntityReference reference) : nEntityComponent(reference)
        {}

        nCombatComponent::nCombatComponent(const ::Inscription::Table<nCombatComponent>& table) : INSCRIPTION_TABLE_GET_BASE(nEntityComponent)
        {}

        void nCombatComponent::SetClass(CharacterClassReference set)
        {
            ATMOS_ASSERT_MESSAGE(set.Get(), "This registry object MUST be set before-hand");

            ItemStashSize prevPermItemCount = characterClass->permanentMaxItemCount;
            ItemStashSize prevTempItemCount = characterClass->temporaryMaxItemCount;

            characterClass = set;
            stats.SetCharacterClass(set);

            // Remove all items that are going to be "lost"
            // Permanent
            auto avatarInventory = Manager()->FindSystem<Ent::nEntityAvatarSystem>()->Avatar()->Component<Ent::nInventoryComponent>();
            {
                auto loop = permanentStash.rbegin();
                while (permanentStash.size() > characterClass->permanentMaxItemCount)
                {
                    avatarInventory->Add(*loop, ItemStack::Count(1));
                    permanentStash.erase(loop);
                }
            }
        }

        bool nCombatComponent::IsDead() const
        {
            return resources.GetHealth() == 0;
        }

        bool nCombatComponent::GiveItem(ItemReference item)
        {
            if (!HasSpaceInStash())
                return false;

            permanentStash.AddBack(item.Get());
            return true;
        }

        bool nCombatComponent::RemoveItem(ItemStashSize pos)
        {
            if (permanentStash.empty() || permanentStash.size() - 1 < pos)
                return false;

            permanentStash.Remove(pos);
            return true;
        }

        nCombatComponent::ItemReference nCombatComponent::FindItem(ItemStashSize pos)
        {
            auto found = permanentStash.Find(pos);
            if (found == permanentStash.end())
                return nullptr;

            return *found;
        }

        bool nCombatComponent::HasSpaceInStash() const
        {
            return permanentStash.size() < characterClass->permanentMaxItemCount;
        }

        void nCombatComponent::Equip(EquipSlot slot, ItemReference item)
        {

        }

        void nCombatComponent::Unequip(EquipSlot slot)
        {

        }

        nCombatComponent::ItemReference nCombatComponent::GetEquipment(EquipSlot slot) const
        {
            auto found = equipment.find(slot);
            if (found == equipment.end())
                return ItemReference();

            return found->second;
        }

        bool nCombatComponent::CanEquip(EquipSlot slot) const
        {
            return equipment.find(slot) == equipment.end();
        }

        bool nCombatComponent::LearnSpell(SpellReference learn)
        {
            if (!CanLearnSpell(learn))
                return false;

            spells.emplace(learn->name, learn);
            return true;
        }

        bool nCombatComponent::CanLearnSpell(SpellReference test) const
        {
            if (!characterClass)
                return false;

            auto found = std::find(test->allowedClasses.begin(), test->allowedClasses.end(), characterClass);
            return found != test->allowedClasses.end();
        }

        Defense nCombatComponent::GetDefense() const
        {
            Defense ret = 0;
            for (auto& loop : equipment)
            {
                //ATMOS_ASSERT_MESSAGE(loop.second->equippableAspect, "Everything equipped must have an equippable aspect.");
                //ret += loop.second->equippableAspect->defense;
            }

            return ret;
        }

        ObjectTypeDescription nCombatComponent::TypeDescription() const
        {
            return ObjectTraits<nCombatComponent>::TypeDescription();
        }
    }

    const ObjectTypeName ObjectTraits<Ent::nCombatComponent>::typeName = "CombatComponent";
}

namespace Inscription
{
    DEFINE_OBJECT_INSCRIPTER_MEMBERS(::Atmos::Ent::nCombatComponent)
    {
        INSCRIPTION_TABLE_ADD(stats);
        INSCRIPTION_TABLE_ADD(resources);
        INSCRIPTION_TABLE_ADD(acumen);
        INSCRIPTION_TABLE_ADD(proficiencies);
        INSCRIPTION_TABLE_ADD(effectiveness);
        INSCRIPTION_TABLE_ADD(characterClass);
        INSCRIPTION_TABLE_ADD(permanentStash);
        INSCRIPTION_TABLE_ADD(equipment);
        INSCRIPTION_TABLE_ADD(currentExp);
        INSCRIPTION_TABLE_ADD(expToNextLevel);
        INSCRIPTION_TABLE_ADD(level);
        INSCRIPTION_TABLE_ADD(movementRange);
    }

    INSCRIPTION_INSCRIPTER_DEFINE_SERIALIZE_FUNCTION(::Atmos::Ent::nCombatComponent)
    {
        INSCRIPTION_INSCRIPTER_CALL_BASE_SERIALIZE_FUNCTION;

        if (scribe.IsOutput())
        {
            ::Inscription::ContainerSize size(obj.spells.size());
            scribe(size);

            for (auto& loop : obj.spells)
                scribe.Save(loop.second);
        }
        else
        {
            ::Inscription::ContainerSize size;
            scribe.Load(size);

            while (size-- > 0)
            {
                ManagedT::SpellReference spell;
                scribe.Load(spell);

                obj.spells.emplace(spell->name, spell);
            }

            if (obj.characterClass)
            {
                obj.stats.SetCharacterClass(*obj.characterClass);
                obj.resources.SetCharacterClass(*obj.characterClass);
            }
        }
    };
}