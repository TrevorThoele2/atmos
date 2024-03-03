#pragma once

#include "EntityComponent.h"

#include "CharacterClass.h"
#include "Item.h"
#include "Spell.h"
#include "StatAttribute.h"
#include "ResourceAttribute.h"
#include "Acumen.h"
#include "CombatEffectiveness.h"
#include "MovementRange.h"
#include "Defense.h"
#include "EquipSlot.h"

#include "RandomAccessSequence.h"

#include "ObjectSerialization.h"

namespace Atmos
{
    namespace Entity
    {
        class CombatComponent : public Component
        {
        public:
            typedef TypedObjectReference<nCharacterClass> CharacterClassReference;
            typedef TypedObjectReference<nItem> ItemReference;
            typedef TypedObjectReference<nSpell> SpellReference;
        public:
            // Character class
            CharacterClassReference characterClass;

            // Stats and resources
            StatAttributeTable stats;
            ResourceAttributeTable resources;
            Acumen acumen;
            Proficiencies proficiencies;
            CombatEffectiveness effectiveness;

            // Items
            ItemStash permanentStash;

            // Equipment
            std::unordered_map<EquipSlot, ItemReference> equipment;

            // EXP and level
            typedef unsigned int EXP;
            EXP currentExp, expToNextLevel;
            typedef StaticBoundedNumber<unsigned char, 1, 30> Level;
            Level level;

            // Movement range
            MovementRange movementRange;

            // Spells
            typedef std::unordered_map<String, SpellReference> Spells;
            Spells spells;
        public:
            CombatComponent(ObjectManager& manager, EntityReference reference);
            CombatComponent(const CombatComponent& arg) = default;
            CombatComponent(const ::Inscription::Table<CombatComponent>& table);

            void SetClass(CharacterClassReference set);
            bool IsDead() const;

            // Returns true if the item was given
            bool GiveItem(ItemReference item);
            // Returns true if the item was removed
            bool RemoveItem(ItemStashSize pos);
            ItemReference FindItem(ItemStashSize pos);
            bool HasSpaceInStash() const;

            void Equip(EquipSlot slot, ItemReference item);
            void Unequip(EquipSlot slot);
            ItemReference GetEquipment(EquipSlot slot) const;
            bool CanEquip(EquipSlot slot) const;

            bool LearnSpell(SpellReference learn);
            bool CanLearnSpell(SpellReference test) const;

            Defense GetDefense() const;

            ObjectTypeDescription TypeDescription() const override;
        private:
            INSCRIPTION_ACCESS;
        };
    }

    template<>
    struct ObjectTraits<Entity::CombatComponent> : ObjectTraitsBase<Entity::CombatComponent>
    {
        static const ObjectTypeName typeName;
    };
}

namespace Inscription
{
    DECLARE_OBJECT_INSCRIPTER(::Atmos::Entity::CombatComponent)
    {
    public:
        OBJECT_INSCRIPTER_DECLARE_MEMBERS;

        INSCRIPTION_INSCRIPTER_DECLARE_SERIALIZE_FUNCTION;
    };
}