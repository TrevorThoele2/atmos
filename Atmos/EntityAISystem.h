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
            INSCRIPTION_BINARY_TABLE_CONSTRUCTOR_DECLARE(AISystem);
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
        INSCRIPTION_BINARY_INSCRIPTER_DECLARE_TABLE;
        INSCRIPTION_BINARY_DECLARE_CLASS_NAME_RESOLVER;
    };
}