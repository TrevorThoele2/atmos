#pragma once

#include "ObjectSystem.h"

#include "ObjectBatch.h"

#include "AIComponent.h"

namespace Atmos::Entity
{
    class AISystem : public ObjectSystem
    {
    public:
        AISystem(ObjectManager& manager);
    protected:
        void WorkImpl() override;
    private:
        typedef ObjectBatch<AIComponent> AIBatch;
        AIBatch aiComponents;
    };
}

namespace Inscription
{
    template<>
    class Scribe<::Atmos::Entity::AISystem, BinaryArchive> :
        public ObjectSystemScribe<::Atmos::Entity::AISystem, BinaryArchive>
    {
    protected:
        void ScrivenImplementation(ObjectT& object, ArchiveT& archive) override;
    };
}