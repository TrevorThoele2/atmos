
#include "QuestSystem.h"

namespace Atmos
{
    QuestSystem::QuestSystem(ObjectManager& manager) : RegistryObjectSystem(manager)
    {}

    QuestSystem::QuestSystem(const ::Inscription::Table<QuestSystem>& table) : INSCRIPTION_TABLE_GET_BASE(RegistryObjectSystem)
    {}
}

namespace Inscription
{
    INSCRIPTION_INSCRIPTER_DEFINE_TABLE(::Atmos::QuestSystem)
    {
        INSCRIPTION_INSCRIPTER_CREATE_TABLE;
        INSCRIPTION_INSCRIPTER_RETURN_TABLE;
    }

    INSCRIPTION_DEFINE_SIMPLE_CLASS_NAME_RESOLVER(::Atmos::QuestSystem, "QuestSystem");
}