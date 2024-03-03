
#include "ItemRecipe.h"

namespace Atmos
{
    INSCRIPTION_SERIALIZE_FUNCTION_DEFINE(ItemRecipe)
    {
        scribe(result);
    }

    ItemRecipe::ItemRecipe(const Name& name) : RegistryObject(name)
    {}
}