
#include "ShaderSystem.h"

namespace Atmos
{
    ShaderSystem::ShaderSystem(ObjectManager& manager) : AssetSystem(manager)
    {}

    ShaderSystem::ShaderSystem(const ::Inscription::Table<ShaderSystem>& table) : INSCRIPTION_TABLE_GET_BASE(AssetSystem)
    {}
}

namespace Inscription
{
    INSCRIPTION_INSCRIPTER_DEFINE_TABLE(::Atmos::ShaderSystem)
    {
        INSCRIPTION_INSCRIPTER_CREATE_TABLE;
        INSCRIPTION_INSCRIPTER_RETURN_TABLE;
    }

    INSCRIPTION_DEFINE_SIMPLE_CLASS_NAME_RESOLVER(::Atmos::ShaderSystem, "ShaderSystem");
}