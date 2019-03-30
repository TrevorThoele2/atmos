
#include "ItemDrop.h"

#include "Item.h"
#include "Item.h"
#include "Random.h"

#include "AvatarSystem.h"
#include "InventoryComponent.h"

#include "CurrentField.h"

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

    ItemDrop::ItemDrop(ItemReference item, CountT count, ChanceT chance) : item(item), count(count), chance(chance)
    {}

    void ItemDrop::AttemptDrop()
    {
        /*
        auto shouldDrop = Random::Generate(FixedPoint64(0), FixedPoint64(100)) <= chance;
        if (shouldDrop)
        {
            auto avatarSystem = GetLocalObjectManager()->FindSystem<Ent::nEntityAvatarSystem>();
            avatarSystem->Avatar()->Component<Ent::nInventoryComponent>()->Add(item, ItemStack::Count(count));
        }
        */
    }
}