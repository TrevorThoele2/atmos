
#include "ItemStack.h"

namespace Atmos
{
    ItemStack::ItemStack() : itemSource(), count(0)
    {}

    ItemStack::ItemStack(ItemReference source, Count count) : itemSource(source), count(count)
    {}

    bool ItemStack::operator==(const ItemStack &arg) const
    {
        return itemSource == arg.itemSource && count == arg.count;
    }

    bool ItemStack::operator!=(const ItemStack &arg) const
    {
        return !(*this == arg);
    }

    INSCRIPTION_SERIALIZE_FUNCTION_DEFINE(ItemStack)
    {
        scribe(itemSource);
        scribe(count);
    }
}