#pragma once

#include "RegistryObject.h"

#include "Item.h"

#include "Serialization.h"

namespace Atmos
{
    class ItemRecipe : public RegistryObject
    {
    private:
        INSCRIPTION_SERIALIZE_FUNCTION_DECLARE;
        INSCRIPTION_ACCESS;
    public:
        RegistryObjectReference<Item> result;

        ItemRecipe();
        ItemRecipe(const ItemRecipe &arg) = default;
        ItemRecipe& operator=(const ItemRecipe &arg) = default;
        ItemRecipe(ItemRecipe &&arg);
        ItemRecipe& operator=(ItemRecipe &&arg);
    };

    template<>
    class Registry<ItemRecipe> : public RegistryBase<ItemRecipe, Registry<ItemRecipe>>
    {
    private:
        Registry() = default;
        friend RegistryBase<ItemRecipe, Registry<ItemRecipe>>;
    };
}