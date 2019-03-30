#pragma once

#include "Item.h"

#include "BoundedNumber.h"

#include "StoredProperty.h"

#include "Serialization.h"

namespace Atmos
{
    class ItemStack
    {
    public:
        typedef TypedObjectReference<nItem> ItemReference;
        typedef StoredProperty<ItemReference> ItemProperty;
        ItemProperty itemSource;
    public:
        typedef StaticBoundedNumber<unsigned char, 0, 99> Count;
        static constexpr Count::ValueT maxCount = Count::UpperBoundStatic();
        Count count;
    public:
        ItemStack();
        ItemStack(ItemReference itemSource, Count count);
        ItemStack(const ItemStack &arg) = default;
        bool operator==(const ItemStack &arg) const;
        bool operator!=(const ItemStack &arg) const;
    private:
        INSCRIPTION_SERIALIZE_FUNCTION_DECLARE;
        INSCRIPTION_ACCESS;
    };
}