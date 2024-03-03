
#include "ItemSystem.h"

namespace Atmos
{
    ItemSystem::ItemSystem(ObjectManager& manager) : RegistryObjectSystem(manager)
    {}

    ItemSystem::ItemSystem(const ::Inscription::Table<ItemSystem>& table) : INSCRIPTION_TABLE_GET_BASE(RegistryObjectSystem)
    {}
}

namespace Inscription
{
    INSCRIPTION_INSCRIPTER_DEFINE_TABLE(::Atmos::ItemSystem)
    {
        INSCRIPTION_INSCRIPTER_CREATE_TABLE;
        INSCRIPTION_INSCRIPTER_RETURN_TABLE;
    }

    INSCRIPTION_DEFINE_SIMPLE_CLASS_NAME_RESOLVER(::Atmos::ItemSystem, "ItemSystem");
}