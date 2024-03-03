
#include "EntitySystem.h"

#include "ObjectManager.h"
#include "DebugStatisticsSystem.h"

#include "StringUtility.h"

namespace Atmos::Entity
{
    EntitySystem::EntitySystem(ObjectManager& manager) : ObjectSystem(manager)
    {
        entityBatch = manager.Batch<Entity>();

        generalBatch = manager.Batch<GeneralComponent>();

        componentBatch = manager.Batch<Component>();
        componentBatch.onCreated.Subscribe(&EntitySystem::OnEntityComponentCreated, *this);
        componentBatch.onBeforeDestroyed.Subscribe(&EntitySystem::OnEntityComponentDestroyed, *this);
    }

    EntitySystem::EntityReference EntitySystem::EntityWithName(const Name& name) const
    {
        for (auto& loop : generalBatch)
            if (loop->name == name)
                return loop->owner;

        return EntityReference();
    }

    ObjectBatchSizeT EntitySystem::Size() const
    {
        return generalBatch.Size();
    }

    void EntitySystem::InitializeImpl()
    {
        auto debugStatistics = Manager()->FindSystem<DebugStatisticsSystem>();
        debugStatistics->memoryPage.entitySize.retrievalFunction = [this]() -> String
        {
            return ToString(Size());
        };
    }

    void EntitySystem::OnEntityComponentCreated(ComponentReference created)
    {
        created->owner->componentList.push_back(created);
    }

    void EntitySystem::OnEntityComponentDestroyed(ComponentReference destroyed)
    {
        for (auto loop = destroyed->owner->componentList.begin(); loop != destroyed->owner->componentList.end(); ++loop)
        {
            if (*loop != destroyed)
                continue;

            destroyed->owner->componentList.erase(loop);
            return;
        }
    }
}

namespace Inscription
{
    void Scribe<::Atmos::Entity::EntitySystem, BinaryArchive>::ScrivenImplementation(ObjectT& object, ArchiveT& archive)
    {
        BaseScriven<::Atmos::ObjectSystem>(object, archive);
    }
}