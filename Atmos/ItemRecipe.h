#pragma once

#include "Item.h"
#include "ObjectReference.h"

#include "Serialization.h"

namespace Atmos
{
    class ItemRecipe : public RegistryObject
    {
    private:
        INSCRIPTION_SERIALIZE_FUNCTION_DECLARE;
        INSCRIPTION_ACCESS;
    public:
        TypedObjectReference<nItem> result;

        ItemRecipe(const Name& name);
        ItemRecipe(const ItemRecipe &arg) = default;
        ItemRecipe& operator=(const ItemRecipe &arg) = default;
    };
}