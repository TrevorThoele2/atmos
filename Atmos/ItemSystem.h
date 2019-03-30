#pragma once

#include "RegistryObjectSystem.h"

#include "Item.h"

#include "Serialization.h"

namespace Atmos
{
    class ItemSystem : public RegistryObjectSystem<nItem>
    {
    public:
        ItemSystem(ObjectManager& manager);
        ItemSystem(const ::Inscription::Table<ItemSystem>& table);
    };
}

namespace Inscription
{
    INSCRIPTION_INSCRIPTER_DECLARE(::Atmos::ItemSystem)
    {
    public:
        INSCRIPTION_INSCRIPTER_DECLARE_TABLE;
        INSCRIPTION_DECLARE_CLASS_NAME_RESOLVER;
    };
}