#pragma once

#include "ObjectSystem.h"

#include "ObjectBatch.h"

#include "AIComponent.h"

namespace Atmos
{
    namespace Ent
    {
        class nEntityAISystem : public ObjectSystem
        {
        public:
            nEntityAISystem(ObjectManager& manager);
        private:
            void WorkImpl() override;
        private:
            typedef ObjectBatch<nAIComponent> AIBatch;
            AIBatch aiComponents;
        };
    }
}