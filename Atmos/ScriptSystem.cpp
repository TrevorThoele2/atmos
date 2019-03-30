
#include "ScriptSystem.h"

namespace Atmos
{
    ScriptSystem::ScriptSystem(ObjectManager& manager) : AssetSystem(manager)
    {}

    ScriptSystem::ScriptSystem(const ::Inscription::Table<ScriptSystem>& table) : INSCRIPTION_TABLE_GET_BASE(AssetSystem)
    {}
}

namespace Inscription
{
    INSCRIPTION_INSCRIPTER_DEFINE_TABLE(::Atmos::ScriptSystem)
    {
        INSCRIPTION_INSCRIPTER_CREATE_TABLE;
        INSCRIPTION_INSCRIPTER_RETURN_TABLE;
    }

    INSCRIPTION_DEFINE_SIMPLE_CLASS_NAME_RESOLVER(::Atmos::ScriptSystem, "ScriptSystem");
}