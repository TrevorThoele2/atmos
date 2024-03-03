
#include "Item.h"
#include <Inscription/Scribe.h>

namespace Atmos
{
    INSCRIPTION_SERIALIZE_FUNCTION_DEFINE(Item)
    {
        ::inscription::BaseSerialize<RegistryObject>(scribe, *this);
        scribe(flags);
        scribe(consumableAspect);
        scribe(equippableAspect);
        scribe(buyingPrice);
        scribe(sellingPrice);
        scribe(portrait);
    }

    Item::Item(Item &&arg) : RegistryObject(std::move(arg)), flags(std::move(arg.flags)), consumableAspect(std::move(arg.consumableAspect)), equippableAspect(std::move(arg.equippableAspect)), buyingPrice(std::move(arg.buyingPrice)), sellingPrice(std::move(arg.sellingPrice)), portrait(std::move(arg.portrait))
    {}

    Item& Item::operator=(Item &&arg)
    {
        RegistryObject::operator=(std::move(arg));
        flags = std::move(arg.flags);
        consumableAspect = std::move(arg.consumableAspect);
        equippableAspect = std::move(arg.equippableAspect);
        buyingPrice = std::move(arg.buyingPrice);
        sellingPrice = std::move(arg.sellingPrice);
        portrait = std::move(arg.portrait);
        return *this;
    }

    bool Item::operator==(const Item &arg) const
    {
        return RegistryObject::operator==(arg);
    }

    bool Item::operator!=(const Item &arg) const
    {
        return !(*this == arg);
    }

    void Item::SignalConsumableAspect(bool signal)
    {
        if (signal && !HasConsumableAspect())
            consumableAspect.Set(ConsumableAspect());
        else
            consumableAspect.Reset();
    }

    void Item::SignalEquippableAspect(bool signal)
    {
        if (signal && !HasEquippableAspect())
            equippableAspect.Set(EquippableAspect());
        else
            equippableAspect.Reset();
    }

    bool Item::HasConsumableAspect() const
    {
        return consumableAspect.IsValid();
    }

    bool Item::HasEquippableAspect() const
    {
        return equippableAspect.IsValid();
    }

    void Item::SetBuyingPrice(Price set)
    {
        buyingPrice = set;
    }

    void Item::SetSellingPrice(Price set)
    {
        sellingPrice = set;
    }

    Item::Price Item::GetBuyingPrice() const
    {
        return buyingPrice;
    }

    Item::Price Item::GetSellingPrice() const
    {
        return sellingPrice;
    }
}