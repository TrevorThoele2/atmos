
#include "MaterialSystem.h"

namespace Atmos
{
    MaterialSystem::MaterialSystem(ObjectManager& manager) : AssetSystem(manager)
    {}

    MaterialSystem::MaterialSystem(const ::Inscription::Table<MaterialSystem>& table) : INSCRIPTION_TABLE_GET_BASE(AssetSystem)
    {}
}

namespace Inscription
{
    INSCRIPTION_INSCRIPTER_DEFINE_TABLE(::Atmos::MaterialSystem)
    {
        INSCRIPTION_INSCRIPTER_CREATE_TABLE;
        INSCRIPTION_INSCRIPTER_RETURN_TABLE;
    }

    INSCRIPTION_DEFINE_SIMPLE_CLASS_NAME_RESOLVER(::Atmos::MaterialSystem, "MaterialSystem");
}