#pragma once

#include "RegistryObjectSystem.h"

#include "Quest.h"

#include "Serialization.h"

namespace Atmos
{
    class QuestSystem : public RegistryObjectSystem<nQuest>
    {
    public:
        QuestSystem(ObjectManager& manager);
        QuestSystem(const ::Inscription::Table<QuestSystem>& table);
    };
}

namespace Inscription
{
    INSCRIPTION_INSCRIPTER_DECLARE(::Atmos::QuestSystem)
    {
    public:
        INSCRIPTION_INSCRIPTER_DECLARE_TABLE;
        INSCRIPTION_DECLARE_CLASS_NAME_RESOLVER;
    };
}