
/*
#pragma once

#include <unordered_map>

#include "Item.h"
#include "EquipRegion.h"
#include "StatusEffect.h"
#include "CharacterClassGroup.h"

#include "Serialization.h"

namespace Atmos
{
    class Equippable : public Item
    {
    public:
        class Stack : public Item::Stack
        {
        private:
            INSCRIPTION_TABLE_CONSTRUCTOR_SIMPLE_DECLARE(Stack, private);
        private:
            typedef Equippable WrappedT;
        private:
            bool equipped;
            // Slot this is equipped in
            EquipSlot equipSlot;
        protected:
            WrappedT& RetrieveItem() override;
        public:
            Stack(WrappedT &item, const Count &count);
            Stack(const Stack &arg);
            Stack& operator=(const Stack &arg);

            Stack* Clone() const;

            const WrappedT& GetItem() const override;

            void Equipped(EquipSlot slot);
            void Unequipped();

            EquipSlot GetEquipSlot() const;
            bool IsEquipped() const;
            const Element& GetElement() const;
            const EquipRegion& GetEquipRegion() const;
            const CharacterClassGroup& GetClassesCanUse() const;
            bool CanMove() const override;
        };
    private:
        INSCRIPTION_TABLE_CONSTRUCTOR_SIMPLE_DECLARE(Equippable, private);
    private:
        CharacterClassGroup classesCanUse;
        // Where this is allowed to be placed
        EquipRegion equipRegion;
        Element element;
    public:
        Equippable(const std::string &name, const std::string &description, Price buyPrice, Price sellPrice, const CharacterClassGroup &classesCanUse, const EquipRegion &equipRegion, const ElementType &element);
        Equippable(const Equippable &arg);
        Equippable& operator=(const Equippable &arg);
        bool operator==(const Equippable &arg) const;
        Stack* CreateStack(Stack::SizeT count) override;
        Stack::SizeT GetMaxStackSize() const override;

        void SetElement(ElementType set);
        void SetEquipRegion(const EquipRegion &set);
        void SetClassesCanUse(const CharacterClassGroup &set);

        const Element& GetElement() const;
        const EquipRegion& GetEquipRegion() const;
        const CharacterClassGroup& GetClassesCanUse() const;
    };
}

namespace Inscription
{
    template<>
    class Inscripter<::Atmos::Equippable::Stack> : public InscripterBase<::Atmos::Equippable::Stack>
    {
    public:
        INSCRIPTION_INSCRIPTER_DECLARE_TABLE;
        INSCRIPTION_INSCRIPTER_DECLARE_CLASS_NAME_CONTAINER;
    };

    template<>
    class Inscripter<::Atmos::Equippable> : public InscripterBase<::Atmos::Equippable>
    {
    public:
        INSCRIPTION_INSCRIPTER_DECLARE_TABLE;
        INSCRIPTION_INSCRIPTER_DECLARE_CLASS_NAME_CONTAINER;
    };
}
*/