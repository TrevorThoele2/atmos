
#include "ItemRecipe.h"

namespace Atmos
{
    INSCRIPTION_SERIALIZE_FUNCTION_DEFINE(ItemRecipe)
    {
        scribe(result);
    }

    ItemRecipe::ItemRecipe() : result(nullptr)
    {}

    ItemRecipe::ItemRecipe(ItemRecipe &&arg) : result(std::move(arg.result))
    {}

    ItemRecipe& ItemRecipe::operator=(ItemRecipe &&arg)
    {
        result = std::move(arg.result);
        return *this;
    }
}