
#include "Item.h"

namespace Atmos
{
    nItem::nItem(ObjectManager& manager, const Name& name) : RegistryObject(manager, name), buyPrice(0), sellPrice(0)
    {}

    nItem::nItem(const ::Inscription::Table<nItem>& table) : INSCRIPTION_TABLE_GET_BASE(RegistryObject)
    {}

    bool nItem::IsConsumable() const
    {
        return consume.IsOccupied();
    }

    bool nItem::IsEquippable() const
    {
        return equip.IsOccupied();
    }

    bool nItem::CanAttackWith() const
    {
        return attack.IsOccupied();
    }

    ObjectTypeDescription nItem::TypeDescription() const
    {
        return ObjectTraits<nItem>::TypeDescription();
    }

    const ObjectTypeName ObjectTraits<nItem>::typeName = "Item";
}

namespace Inscription
{
    OBJECT_INSCRIPTER_DEFINE_MEMBERS(::Atmos::nItem)
    {

    }
}