
#include "ItemRecipe.h"

namespace Atmos
{
    ItemRecipe::ItemRecipe(ObjectManager& manager, const Name& name) : RegistryObject(manager, name)
    {}

    INSCRIPTION_SERIALIZE_FUNCTION_DEFINE(ItemRecipe)
    {
        scribe(result);
    }
}