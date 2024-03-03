#pragma once

#include "EntityComponent.h"

#include "StatAttribute.h"
#include "ResourceAttribute.h"
#include "Acumen.h"
#include "CharacterClass.h"
#include "Spell.h"
#include "Item.h"
#include "ItemStash.h"
#include "EquipSlot.h"
#include "Defense.h"
#include "Proficiency.h"
#include "CombatEffectiveness.h"
#include "MovementRange.h"
#include "Element.h"

#include "RandomAccessSequence.h"
#include "Optional.h"
#include "ObjectMenuTraits.h"

#include "Serialization.h"

namespace Atmos
{
    namespace Ent
    {
        class CombatComponent : public Component<CombatComponent>
        {
        private:
            INSCRIPTION_SERIALIZE_FUNCTION_DECLARE;
            INSCRIPTION_ACCESS;
        public:
            // Character class
            RegistryObjectReference<CharacterClass> charClass;

            // Stats and resources
            StatAttributeTable stats;
            ResourceAttributeTable resources;
            Acumen acumen;
            Proficiencies proficiencies;
            CombatEffectiveness effectiveness;

            // Items
            ItemStash permanentStash;

            // Equipment
            std::unordered_map<EquipSlot, RegistryObjectReference<Item>> equipment;

            // EXP and level
            typedef unsigned int EXP;
            EXP currentExp, expToNextLevel;
            typedef StaticBoundedNumber<unsigned char, 1, 30> Level;
            Level level;

            // Movement range
            MovementRange movementRange;

            // Spells
            typedef std::unordered_map<String, RegistryObjectReference<Spell>> Spells;
            Spells spells;

            CombatComponent() = default;
            CombatComponent(const CombatComponent &arg) = default;
            CombatComponent(CombatComponent &&arg);
            CombatComponent& operator=(const CombatComponent &arg);
            CombatComponent& operator=(CombatComponent &&arg);

            bool operator==(const CombatComponent &arg) const;
            bool operator!=(const CombatComponent &arg) const;

            void SetClass(const RegistryObjectReference<CharacterClass> &set);
            bool IsCorpse() const;

            // Returns true if the item was given
            bool GiveItem(const RegistryObjectReference<Item> &item);
            // Returns true if the item was removed
            bool RemoveItem(ItemStashSize pos);
            RegistryObjectReference<Item> FindItem(ItemStashSize pos);
            bool HasSpaceInStash() const;

            void Equip(EquipSlot slot, const RegistryObjectReference<Item> &item);
            void Unequip(EquipSlot slot);
            RegistryObjectReference<Item> GetEquipment(EquipSlot slot) const;
            bool CanEquip(EquipSlot slot) const;

            bool LearnSpell(const RegistryObjectReference<Spell> &learn);
            bool CanLearnSpell(const RegistryObjectReference<Spell> &test) const;

            Defense GetDefense() const;
            Element GetElement() const;
        };

        ENTITY_COMPONENT_MAP_DECLARE("Combat", CombatComponent)
    }

    template<>
    struct ObjectMenuTraits<Ent::CombatComponent>
    {
        static Ent::CombatComponent::Spells& RetrieveContainer(Ent::CombatComponent &list) { return list.spells; }
        static const Spell& Unpack(Ent::CombatComponent::Spells::const_iterator itr) { return *itr->second; }
    };
}