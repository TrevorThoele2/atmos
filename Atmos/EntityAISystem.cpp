
#include "EntityAISystem.h"

#include "RunningScript.h"

#include "ObjectManager.h"

namespace Atmos::Entity
{
    AISystem::AISystem(ObjectManager& manager) : ObjectSystem(manager)
    {
        aiComponents = manager.Batch<AIComponent>();
    }

    void AISystem::WorkImpl()
    {
        for (auto& loop : aiComponents)
        {
            auto running = loop->script->RunningForThis();
            if (!running.IsOccupied())
                continue;

            running->Resume();
        }
    }
}

namespace Inscription
{
    void Scribe<::Atmos::Entity::AISystem, BinaryArchive>::ScrivenImplementation(ObjectT& object, ArchiveT& archive)
    {
        BaseScriven<::Atmos::ObjectSystem>(object, archive);
    }
}