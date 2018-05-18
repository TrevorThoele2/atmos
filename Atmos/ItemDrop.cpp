
#include "ItemDrop.h"

#include "Item.h"
#include "Item.h"
#include "Random.h"

#include "AvatarSystem.h"
#include "InventoryComponent.h"

#include <Inscription\Inscripter.h>
#include <Inscription\Memory.h>

namespace Atmos
{
    INSCRIPTION_SERIALIZE_FUNCTION_DEFINE(ItemDrop)
    {
        scribe(item);
        scribe(count);
        scribe(chance);
    }

    ItemDrop::ItemDrop() : item(nullptr), count(0), chance(0.0)
    {}

    ItemDrop::ItemDrop(RegistryObjectReference<Item> item, CountT count, ChanceT chance) : item(item), count(count), chance(chance)
    {}

    void ItemDrop::AttemptDrop()
    {
        if (Random::Generate(FixedPoint64(0), FixedPoint64(100)) <= chance)
            ::Atmos::Ent::AvatarSystem::GetInventory()->Add(item->GetName(), ItemStack::CountT(count));
    }
}