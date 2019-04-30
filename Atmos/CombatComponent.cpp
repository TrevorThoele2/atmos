
#include "CombatComponent.h"

#include "Battle.h"
#include "EntityAvatarSystem.h"
#include "InventoryComponent.h"

#include <Inscription\UnorderedMap.h>
#include <Inscription\Memory.h>

#include "Assert.h"

namespace Atmos
{
    namespace Entity
    {
        CombatComponent::CombatComponent(ObjectManager& manager, EntityReference reference) :
            Component(manager, reference)
        {}

        CombatComponent::CombatComponent(const ::Inscription::Table<CombatComponent>& table) : INSCRIPTION_TABLE_GET_BASE(Component)
        {}

        void CombatComponent::SetClass(CharacterClassReference set)
        {
            ATMOS_ASSERT_MESSAGE(set.Get(), "This registry object MUST be set before-hand");

            ItemStashSize prevPermItemCount = characterClass->permanentMaxItemCount;
            ItemStashSize prevTempItemCount = characterClass->temporaryMaxItemCount;

            characterClass = set;
            stats.SetCharacterClass(set);

            // Remove all items that are going to be "lost"
            // Permanent
            auto avatarInventory = Manager()->FindSystem<AvatarSystem>()->Avatar()->RetrieveComponent<InventoryComponent>();
            {
                auto loop = permanentStash.rbegin();
                while (permanentStash.size() > characterClass->permanentMaxItemCount)
                {
                    avatarInventory->Add(*loop, ItemStack::Count(1));
                    permanentStash.erase(loop);
                }
            }
        }

        bool CombatComponent::IsDead() const
        {
            return resources.GetHealth() == 0;
        }

        bool CombatComponent::GiveItem(ItemReference item)
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

        CombatComponent::ItemReference CombatComponent::FindItem(ItemStashSize pos)
        {
            auto found = permanentStash.Find(pos);
            if (found == permanentStash.end())
                return nullptr;

            return *found;
        }

        bool CombatComponent::HasSpaceInStash() const
        {
            return permanentStash.size() < characterClass->permanentMaxItemCount;
        }

        void CombatComponent::Equip(EquipSlot slot, ItemReference item)
        {

        }

        void CombatComponent::Unequip(EquipSlot slot)
        {

        }

        CombatComponent::ItemReference CombatComponent::GetEquipment(EquipSlot slot) const
        {
            auto found = equipment.find(slot);
            if (found == equipment.end())
                return ItemReference();

            return found->second;
        }

        bool CombatComponent::CanEquip(EquipSlot slot) const
        {
            return equipment.find(slot) == equipment.end();
        }

        bool CombatComponent::LearnSpell(SpellReference learn)
        {
            if (!CanLearnSpell(learn))
                return false;

            spells.emplace(learn->name, learn);
            return true;
        }

        bool CombatComponent::CanLearnSpell(SpellReference test) const
        {
            if (!characterClass)
                return false;

            auto found = std::find(test->allowedClasses.begin(), test->allowedClasses.end(), characterClass);
            return found != test->allowedClasses.end();
        }

        Defense CombatComponent::GetDefense() const
        {
            Defense ret = 0;
            for (auto& loop : equipment)
            {
                //ATMOS_ASSERT_MESSAGE(loop.second->equippableAspect, "Everything equipped must have an equippable aspect.");
                //ret += loop.second->equippableAspect->defense;
            }

            return ret;
        }

        ObjectTypeDescription CombatComponent::TypeDescription() const
        {
            return ObjectTraits<CombatComponent>::TypeDescription();
        }
    }

    const ObjectTypeName ObjectTraits<Entity::CombatComponent>::typeName = "CombatComponent";
}

namespace Inscription
{
    OBJECT_INSCRIPTER_DEFINE_MEMBERS(::Atmos::Entity::CombatComponent)
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

    INSCRIPTION_INSCRIPTER_DEFINE_SERIALIZE_FUNCTION(::Atmos::Entity::CombatComponent)
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