
#pragma once

#include "Item.h"
#include "ItemStack.h"

#include "RegistryObjectReference.h"
#include "FixedPoint.h"

#include "Serialization.h"

namespace Atmos
{
    class ItemDrop
    {
    public:
        typedef double ChanceT;
        typedef ItemStack::CountT::ValueT CountT;
    private:
        INSCRIPTION_SERIALIZE_FUNCTION_DECLARE;
        INSCRIPTION_ACCESS;
    private:
        RegistryObjectReference<Item> item;
        CountT count;
        FixedPoint64 chance;
    public:
        ItemDrop();
        ItemDrop(RegistryObjectReference<Item> item, CountT count, ChanceT chance);
        ItemDrop(const ItemDrop &arg) = default;
        ItemDrop& operator=(const ItemDrop &arg) = default;

        void AttemptDrop();
    };
}