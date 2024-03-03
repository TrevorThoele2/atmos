#pragma once

#include "Item.h"
#include "ObjectReference.h"

#include "Serialization.h"

namespace Atmos
{
    class ItemRecipe : public RegistryObject
    {
    public:
        TypedObjectReference<nItem> result;

        ItemRecipe(ObjectManager& manager, const Name& name);
        ItemRecipe(const ItemRecipe &arg) = default;
        ItemRecipe& operator=(const ItemRecipe &arg) = default;
    private:
        INSCRIPTION_SERIALIZE_FUNCTION_DECLARE;
        INSCRIPTION_ACCESS;
    };
}