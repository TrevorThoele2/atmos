
#pragma once

#include "Item.h"
#include "ItemStack.h"

#include "FixedPoint.h"

#include "Serialization.h"

namespace Atmos
{
    class nItem;

    class ItemDrop
    {
    public:
        typedef TypedObjectReference<nItem> ItemReference;

        typedef double ChanceT;
        typedef ItemStack::Count::ValueT CountT;
    private:
        INSCRIPTION_SERIALIZE_FUNCTION_DECLARE;
        INSCRIPTION_ACCESS;
    private:
        ItemReference item;
        CountT count;
        FixedPoint64 chance;
    public:
        ItemDrop();
        ItemDrop(ItemReference item, CountT count, ChanceT chance);
        ItemDrop(const ItemDrop &arg) = default;
        ItemDrop& operator=(const ItemDrop &arg) = default;

        void AttemptDrop();
    };
}