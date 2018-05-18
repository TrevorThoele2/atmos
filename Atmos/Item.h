#pragma once

#include "RegistryObject.h"

#include "ConsumableAspect.h"
#include "EquippableAspect.h"

#include "Sprite.h"
#include "Flags.h"
#include "Optional.h"

#include "Serialization.h"

namespace Atmos
{
    class Item : public RegistryObject
    {
    public:
        enum class Flag
        {
            KEY = 1 << 0,
            TEMPORARY = 1 << 1
        };

        typedef unsigned int Price;
    private:
        INSCRIPTION_SERIALIZE_FUNCTION_DECLARE;
        INSCRIPTION_ACCESS;
    public:
        Flags<Flag> flags;

        typedef Optional<ConsumableAspect> OptionalConsumable;
        typedef Optional<EquippableAspect> OptionalEquippable;

        OptionalConsumable consumableAspect;
        OptionalEquippable equippableAspect;

        Price buyingPrice;
        Price sellingPrice;

        Sprite portrait;

        Item() = default;
        Item(const Item &arg) = default;
        Item& operator=(const Item &arg) = default;
        Item(Item &&arg);
        Item& operator=(Item &&arg);

        bool operator==(const Item &arg) const;
        bool operator!=(const Item &arg) const;

        // If pass false for signal, will reset the consumable aspect
        // Will not reset the aspect if it already exists
        void SignalConsumableAspect(bool signal = true);
        // If pass false for signal, will reset the equippable aspect
        // Will not reset the aspect if it already exists
        void SignalEquippableAspect(bool signal = true);

        bool HasConsumableAspect() const;
        bool HasEquippableAspect() const;

        void SetBuyingPrice(Price set);
        void SetSellingPrice(Price set);
        Price GetBuyingPrice() const;
        Price GetSellingPrice() const;
    };

    template<>
    class Registry<Item> : public RegistryBase<Item, Registry<Item>>
    {
    private:
        Registry() = default;
        friend RegistryBase<Item, Registry<Item>>;
    };

    typedef Registry<Item> ItemRegistry;
}