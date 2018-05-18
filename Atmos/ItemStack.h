#pragma once

#include "Item.h"
#include "BoundedNumber.h"
#include "Serialization.h"

namespace Atmos
{
    class ItemStack
    {
    public:
        typedef Item WrappedT;
        typedef StaticBoundedNumber<unsigned char, 0, 99> CountT;
        static constexpr CountT::ValueT maxCount = CountT::GetUpperBoundStatic();

        typedef WrappedT::Description Description;
    private:
        INSCRIPTION_SERIALIZE_FUNCTION_DECLARE;
        INSCRIPTION_ACCESS;
    private:
        RegistryObjectReference<WrappedT> wrapped;
        CountT count;
    public:
        ItemStack();
        ItemStack(RegistryObjectReference<WrappedT> wrapped, CountT count);
        ItemStack(const ItemStack &arg) = default;
        ItemStack& operator=(const ItemStack &arg) = default;
        ItemStack(ItemStack &&arg);
        ItemStack& operator=(ItemStack &&arg);
        bool operator==(const ItemStack &arg) const;
        bool operator!=(const ItemStack &arg) const;

        const WrappedT& operator*() const;
        const WrappedT* operator->() const;
        
        void SetWrapped(RegistryObjectReference<WrappedT> set);
        RegistryObjectReference<WrappedT> GetWrapped() const;

        void SetCount(CountT set);
        void IncrementCount(CountT inc = CountT(1));
        void DecrementCount(CountT dec = CountT(1));
        CountT GetCount() const;

        const Name& GetName() const;
        const Description& GetDescription() const;
    };
}