
#include "ItemStack.h"

namespace Atmos
{
    INSCRIPTION_SERIALIZE_FUNCTION_DEFINE(ItemStack)
    {
        scribe(wrapped);
        scribe(count);
    }

    ItemStack::ItemStack() : wrapped(wrapped)
    {}

    ItemStack::ItemStack(RegistryObjectReference<WrappedT> wrapped, CountT count) : wrapped(wrapped), count(count)
    {}

    ItemStack::ItemStack(ItemStack &&arg) : wrapped(std::move(arg.wrapped)), count(std::move(arg.count))
    {}

    ItemStack& ItemStack::operator=(ItemStack &&arg)
    {
        wrapped = std::move(arg.wrapped);
        count = std::move(arg.count);
        return *this;
    }

    bool ItemStack::operator==(const ItemStack &arg) const
    {
        return wrapped == arg.wrapped && count == arg.count;
    }

    bool ItemStack::operator!=(const ItemStack &arg) const
    {
        return !(*this == arg);
    }

    const ItemStack::WrappedT& ItemStack::operator*() const
    {
        return *wrapped;
    }

    const ItemStack::WrappedT* ItemStack::operator->() const
    {
        return wrapped.Get();
    }

    void ItemStack::SetWrapped(RegistryObjectReference<WrappedT> set)
    {
        wrapped = set;
    }

    RegistryObjectReference<ItemStack::WrappedT> ItemStack::GetWrapped() const
    {
        return wrapped;
    }

    void ItemStack::SetCount(CountT set)
    {
        count = set;
    }

    void ItemStack::IncrementCount(CountT inc)
    {
        count += inc;
    }

    void ItemStack::DecrementCount(CountT dec)
    {
        count -= dec;
    }

    ItemStack::CountT ItemStack::GetCount() const
    {
        return count;
    }

    const Name& ItemStack::GetName() const
    {
        return wrapped->GetName();
    }

    const ItemStack::Description& ItemStack::GetDescription() const
    {
        return wrapped->GetDescription();
    }
}