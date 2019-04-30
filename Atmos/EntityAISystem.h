#pragma once

#include "ObjectSystem.h"

#include "ObjectBatch.h"

#include "AIComponent.h"

namespace Atmos
{
    namespace Entity
    {
        class AISystem : public ObjectSystem
        {
        public:
            AISystem(ObjectManager& manager);
            AISystem(const ::Inscription::Table<AISystem>& table);
        private:
            void WorkImpl() override;
        private:
            typedef ObjectBatch<AIComponent> AIBatch;
            AIBatch aiComponents;
        };
    }
}

namespace Inscription
{
    INSCRIPTION_INSCRIPTER_DECLARE(::Atmos::Entity::AISystem)
    {
    public:
        INSCRIPTION_INSCRIPTER_DECLARE_TABLE;
        INSCRIPTION_DECLARE_CLASS_NAME_RESOLVER;
    };
}