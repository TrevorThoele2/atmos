
/*
#include "Equippable.h"

#include "Ammo.h"
#include "Entity.h"
#include "InventoryComponent.h"
#include "CombatComponent.h"

#include "CurrentField.h"

#include <Inscription\Inscripter.h>

namespace Atmos
{
    INSCRIPTION_TABLE_CONSTRUCTOR_DEFINE_EX(Equippable::Stack, Stack) : INSCRIPTION_TABLE_GET_BASE(Item::Stack), INSCRIPTION_TABLE_GET_MEM(equipped), INSCRIPTION_TABLE_GET_MEM(equipSlot)
    {}

    Equippable::Stack::WrappedT& Equippable::Stack::RetrieveItem()
    {
        return CastItem<Equippable>();
    }

    Equippable::Stack::Stack(WrappedT &item, const Count &count) : Item::Stack(item, count), equipped(false), equipSlot(EquipSlot::NONE)
    {}

    Equippable::Stack::Stack(const Stack &arg) : Item::Stack(arg)
    {}

    Equippable::Stack& Equippable::Stack::operator=(const Stack &arg)
    {
        Item::Stack::operator=(arg);
        return *this;
    }

    Equippable::Stack* Equippable::Stack::Clone() const
    {
        return new Stack(*this);
    }

    const Equippable::Stack::WrappedT& Equippable::Stack::GetItem() const
    {
        return CastItem<Equippable>();
    }

    void Equippable::Stack::Equipped(EquipSlot slot)
    {
        equipped = true;
        equipSlot = slot;
    }

    void Equippable::Stack::Unequipped()
    {
        equipped = false;
        equipSlot = EquipSlot::NONE;
    }

    EquipSlot Equippable::Stack::GetEquipSlot() const
    {
        return equipSlot;
    }

    bool Equippable::Stack::IsEquipped() const
    {
        return equipped;
    }

    const Element& Equippable::Stack::GetElement() const
    {
        return GetItem().GetElement();
    }

    const EquipRegion& Equippable::Stack::GetEquipRegion() const
    {
        return GetItem().GetEquipRegion();
    }

    const CharacterClassGroup& Equippable::Stack::GetClassesCanUse() const
    {
        return GetItem().GetClassesCanUse();
    }

    bool Equippable::Stack::CanMove() const
    {
        return Item::Stack::CanMove() && !equipped;
    }

    INSCRIPTION_TABLE_CONSTRUCTOR_DEFINE(Equippable) : INSCRIPTION_TABLE_GET_BASE(Item), INSCRIPTION_TABLE_GET_MEM(classesCanUse), INSCRIPTION_TABLE_GET_MEM(equipRegion),
        INSCRIPTION_TABLE_GET_MEM(element)
    {}

    Equippable::Equippable(const std::string &name, const std::string &description, Price buyPrice, Price sellPrice, const CharacterClassGroup &classesCanUse, const EquipRegion &equipRegion, const ElementType &element) : Item(name, description, buyPrice, sellPrice), classesCanUse(classesCanUse), equipRegion(equipRegion), element(element)
    {}

    Equippable::Equippable(const Equippable &arg) : Item(arg), classesCanUse(arg.classesCanUse), equipRegion(arg.equipRegion), element(arg.element)
    {}

    Equippable& Equippable::operator=(const Equippable &arg)
    {
        Item::operator=(arg);
        classesCanUse = arg.classesCanUse;
        equipRegion = arg.equipRegion;
        element = arg.element;
        return *this;
    }

    bool Equippable::operator==(const Equippable &arg) const
    {
        return Item::operator==(arg) && classesCanUse == arg.classesCanUse && equipRegion == arg.equipRegion && element == arg.element;
    }

    Equippable::Stack* Equippable::CreateStack(Stack::SizeT count)
    {
        return new Stack(*this, Stack::Count(GetMaxStackSize(), count));
    }

    Equippable::Stack::SizeT Equippable::GetMaxStackSize() const
    {
        return 1;
    }

    void Equippable::SetElement(ElementType set)
    {
        element = set;
    }

    void Equippable::SetEquipRegion(const EquipRegion &set)
    {
        equipRegion = set;
    }

    void Equippable::SetClassesCanUse(const CharacterClassGroup &set)
    {
        classesCanUse = set;
    }

    const Element& Equippable::GetElement() const
    {
        return element;
    }

    const EquipRegion& Equippable::GetEquipRegion() const
    {
        return equipRegion;
    }

    const CharacterClassGroup& Equippable::GetClassesCanUse() const
    {
        return classesCanUse;
    }
}

namespace Inscription
{
    INSCRIPTION_INSCRIPTER_DEFINE_TABLE(::Atmos::Equippable::Stack)
        INSCRIPTION_TABLE_ADD_BASE(::Atmos::Item::Stack)
        INSCRIPTION_TABLE_ADD(equipped)
        INSCRIPTION_TABLE_ADD(equipSlot)
    INSCRIPTION_TABLE_END

    INSCRIPTION_INSCRIPTER_DEFINE_CLASS_NAME_CONTAINER(::Atmos::Equippable::Stack, "EquippableStack")
    INSCRIPTION_INSCRIPTER_DEFINE_CLASS_NAME_CONTAINER_END

    INSCRIPTION_INSCRIPTER_DEFINE_TABLE(::Atmos::Equippable)
        INSCRIPTION_TABLE_ADD_BASE(::Atmos::Item)
        INSCRIPTION_TABLE_ADD(classesCanUse)
        INSCRIPTION_TABLE_ADD(equipRegion)
        INSCRIPTION_TABLE_ADD(element)
    INSCRIPTION_TABLE_END

    INSCRIPTION_INSCRIPTER_DEFINE_CLASS_NAME_CONTAINER(::Atmos::Equippable, "Equippable")
    INSCRIPTION_INSCRIPTER_DEFINE_CLASS_NAME_CONTAINER_END
}
*/